import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "POST":
        # Ensure username was submitted
        stocks = list(db.execute(
            "SELECT stock, SUM(quantity_buy_sold) as total FROM transactions WHERE user_id = ? GROUP BY stock;", session["user_id"]))
        for stock in stocks:
            stock_info = lookup(stock['stock'])
            if stock_info is not None:
                stock['current_price'] = stock_info['price']
            stock['total_value'] = stock['current_price'] * stock['total']
        wallet = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )
        Total_value = 0
        for stock in stocks:
            Total_value += stock['current_price'] * stock['total']

        for money in wallet:
            money['Total_value_stocks'] = Total_value
        if not request.form.get("stock"):
            unsuccess = f"Must provide a stock symbol"
            return render_template("index.html", unsuccess=unsuccess, stocks=stocks, wallet=wallet, usd=usd), 400

        elif lookup(request.form.get("stock")) is None:
            unsuccess = f"Stock not available"
            return render_template("index.html", unsuccess=unsuccess, stocks=stocks, wallet=wallet, usd=usd), 400
        else:
            stock = lookup(request.form.get("stock"))
        quantity = request.form.get("quantity")
        if not quantity:
            unsuccess = f"Must provide a positive integer value."
            return render_template("index.html", unsuccess=unsuccess, stocks=stocks, wallet=wallet, usd=usd), 400
        elif not (quantity.isdigit() and quantity != 0):
            unsuccess = f"Must provide a positive integer value."
            return render_template("index.html", unsuccess=unsuccess, stocks=stocks, wallet=wallet, usd=usd), 400
        quantity = int(request.form.get("quantity"))

        cash = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )[0]["cash"]
        transaction_amount = stock["price"] * quantity

        if stock["price"] * quantity > cash:
            unsuccess = f"Not enough credit balance to proceed with transaction.<br>Current balance: {
                usd(cash)}.<br>Transaction value: {usd(transaction_amount)}."
            return render_template("index.html", unsuccess=unsuccess, stocks=stocks, wallet=wallet, usd=usd), 400
        else:

            db.execute("UPDATE users SET cash = cash - ? WHERE id = ?",
                       transaction_amount, session["user_id"])
            db.execute("INSERT INTO transactions (user_id, stock, stock_price, quantity_buy_sold, date)"
                       "VALUES (?, ?, ?, ?, ?)", session["user_id"], request.form.get(
                           "stock"), stock["price"], quantity, datetime.now()
                       )

            stocks = list(db.execute(
                "SELECT stock, SUM(quantity_buy_sold) as total FROM transactions WHERE user_id = ? GROUP BY stock;", session["user_id"]))
            for stock in stocks:
                stock_info = lookup(stock['stock'])
                if stock_info is not None:
                    stock['current_price'] = stock_info['price']
                stock['total_value'] = stock['current_price'] * stock['total']
            wallet = db.execute(
                "SELECT cash FROM users WHERE id = ?", session["user_id"]
            )
            Total_value = 0
            for stock in stocks:
                Total_value += stock['current_price'] * stock['total']

            for money in wallet:
                money['Total_value_stocks'] = Total_value
                success = f"Transaction sucessful."
                return render_template("index.html", success=success, stocks=stocks, wallet=wallet, usd=usd)

    else:
        stocks = list(db.execute(
            "SELECT stock, SUM(quantity_buy_sold) as total FROM transactions WHERE user_id = ? GROUP BY stock;", session["user_id"]))
        for stock in stocks:
            stock_info = lookup(stock['stock'])
            if stock_info is not None:
                stock['current_price'] = stock_info['price']
            stock['total_value'] = stock['current_price'] * stock['total']
        wallet = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )
        Total_value = 0
        for stock in stocks:
            Total_value += stock['current_price'] * stock['total']

        for money in wallet:
            money['Total_value_stocks'] = Total_value
        return render_template("index.html", stocks=stocks, wallet=wallet, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("symbol"):
            unsuccess = f"Must provide a stock symbol"
            return render_template("buy.html", unsuccess=unsuccess), 400

        elif lookup(request.form.get("symbol")) is None:
            unsuccess = f"Stock not available"
            return render_template("buy.html", unsuccess=unsuccess), 400
        else:
            stock = lookup(request.form.get("symbol"))
        quantity = request.form.get("shares")
        if not quantity or not quantity.isdigit() or int(quantity) <= 0:
            unsuccess = f"Must provide a positive integer value."
            return render_template("buy.html", unsuccess=unsuccess), 400
        else:
            quantity = int(request.form.get("shares"))


        # Query database for cash
        cash = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )[0]["cash"]
        transaction_amount = stock["price"] * quantity

        if stock["price"] * quantity > cash:
            unsuccess = f"Not enough credit balance to proceed with transaction.<br>Current balance: {
                usd(cash)}.<br>Transaction value: {usd(transaction_amount)}."
            return render_template("buy.html", unsuccess=unsuccess), 400
        else:

            db.execute("UPDATE users SET cash = cash - ? WHERE id = ?",
                       transaction_amount, session["user_id"])
            db.execute("INSERT INTO transactions (user_id, stock, stock_price, quantity_buy_sold, date)"
                       "VALUES (?, ?, ?, ?, ?)", session["user_id"], request.form.get(
                           "symbol"), stock["price"], quantity, datetime.now()
                       )
            stock_quantity = int(db.execute("SELECT SUM(quantity_buy_sold) FROM transactions WHERE user_id = ? AND stock = ?",
                                            session["user_id"], request.form.get("symbol")
                                            )[0]['SUM(quantity_buy_sold)'])
            cash = db.execute(
                "SELECT cash FROM users WHERE id = ?", session["user_id"]
            )[0]["cash"]
            success = f"Transaction sucessful. <br>Current balance: {usd(cash)}.<br>Current stock balance: {
                request.form.get("symbol")} {stock_quantity} shares"
            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id = ? ORDER BY date DESC;", session["user_id"])

    return render_template("history.html", transactions=transactions, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            unsuccess = f"Must provide a username."
            return render_template("login.html", unsuccess=unsuccess), 400

        # Ensure password was submitted
        elif not request.form.get("password"):
            unsuccess = f"Must provide a password"
            return render_template("login.html", unsuccess=unsuccess), 400

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            unsuccess = f"Invalid username and/or password."
            return render_template("login.html", unsuccess=unsuccess), 400

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        # Ensure there is a symbol written
        if not request.form.get("symbol"):
            unsuccess = f"Must provide a stock symbol."
            return render_template("quote.html", unsuccess=unsuccess), 400

        elif lookup(request.form.get("symbol")) is None:
            unsuccess = f"Stock not available."
            return render_template("quote.html", unsuccess=unsuccess), 400
        else:
            quotes = lookup(request.form.get("symbol"))
        return render_template("quoted.html", quotes=quotes, usd=usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            unsuccess = f"Must provide a username."
            return render_template("register.html", unsuccess=unsuccess), 400

        # Ensure password was submitted
        elif not request.form.get("password"):
            unsuccess = f"Must provide a password."
            return render_template("register.html", unsuccess=unsuccess), 400

        elif request.form.get("password") != request.form.get("confirmation"):
            unsuccess = f"Passwords don't match."
            return render_template("register.html", unsuccess=unsuccess), 400

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )
        if len(rows) != 0:
            unsuccess = f"Username already taken"
            return render_template("register.html", unsuccess=unsuccess), 400
        else:
            hashed_password = generate_password_hash(request.form.get("password"))
            db.execute("INSERT INTO users (username, hash)"
                       "VALUES(?, ?)", request.form.get("username"), hashed_password)
            return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Ensure username was submitted

        stock = lookup(request.form.get("symbol"))
        quantity = request.form.get("shares")
        if not quantity or not quantity.isdigit() or int(quantity) <= 0:
            unsuccess = f"Must provide a positive integer value."
            return render_template("sell.html", unsuccess=unsuccess), 400
        else:
            quantity = int(request.form.get("shares"))
        # Query database for cash
        cash = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )[0]["cash"]
        stock_quantity = int(db.execute("SELECT SUM(quantity_buy_sold) FROM transactions WHERE user_id = ? AND stock = ?",
                                        session["user_id"], request.form.get("symbol")
                                        )[0]['SUM(quantity_buy_sold)'])
        if stock_quantity < quantity or stock_quantity <= 0:
            unsuccess = f"Not enough stocks to proceed with transaction.<br>Current stock balance: {
                request.form.get("symbol")} {stock_quantity} stocks."
            return render_template("sell.html", unsuccess=unsuccess), 400
        else:
            transaction_amount = stock["price"] * quantity
            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                       transaction_amount, session["user_id"])
            db.execute("INSERT INTO transactions (user_id, stock, stock_price, quantity_buy_sold, date)"
                       "VALUES (?, ?, ?, ?, ?)", session["user_id"], request.form.get(
                           "symbol"), stock["price"], -quantity, datetime.now()
                       )
            stock_quantity = int(db.execute("SELECT SUM(quantity_buy_sold) FROM transactions WHERE user_id = ? AND stock = ?",
                                            session["user_id"], request.form.get("symbol")
                                            )[0]['SUM(quantity_buy_sold)'])
            cash = db.execute(
                "SELECT cash FROM users WHERE id = ?", session["user_id"]
            )[0]["cash"]
            success = f"Transaction sucessful. <br>Current balance: {usd(cash)}.<br>Current stock balance: {
                request.form.get("symbol")} {stock_quantity} shares"
            return redirect("/")
    else:
        symbols = db.execute("SELECT DISTINCT stock FROM transactions WHERE user_id = ?", session["user_id"])
        # Render the template with the symbols
        return render_template("sell.html", symbols=symbols)

       
