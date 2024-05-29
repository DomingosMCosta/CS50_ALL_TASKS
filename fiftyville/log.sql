/*
-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2023 and that it took place on Humphrey Street.

--  QUERY 1: Used this to get all crimes in that day
--  QUERY 1: SELECT description FROM crime_scene_reports WHERE year = 2023 AND month = 7 AND day = 28;

-- RESULT:

| Vandalism took place at 12:04. No known witnesses.                                                                                                                                                                       |
| Shoplifting took place at 03:01. Two people witnessed the event.                                                                                                                                                         |
| Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted
 today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
| Money laundering took place at 20:30. No known witnesses.                                                                                                                                                                |
| Littering took place at 16:36. No known witnesses.


-- UPDATE 1: 3 witnesses mention the bakery. crime took place at 10:15am

-- QUERY 2: To find all interviews the day of the crime
-- QUERY 2: SELECT name, transcript FROM interviews WHERE year = 2023 AND month = 7 AND day = 28;

-- RESULT:

| Jose    | “Ah,” said he, “I forgot that I had not seen you for some weeks. It is a little souvenir from the
King of Bohemia in return for my assistance in the case of the Irene Adler papers.”
                                                                                                                               |
| Eugene  | “I suppose,” said Holmes, “that when Mr. Windibank came back from France
he was very annoyed at your having gone to the ball.”
                                                                                                                                                                                      |
| Barbara | “You had my note?” he asked with a deep harsh voice and a strongly marked German accent.
“I told you that I would call.” He looked from one to the other of us, as if uncertain which to address.
                                                                                                                   |
| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery
parking lot and drive away. If you have security footage from the bakery parking lot, you might want
to look for cars that left the parking lot in that time frame.
                                                          |
| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning,
before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
                                                                                                 |
| Raymond | As the thief was leaving the bakery, they called someone who talked to them for
less than a minute. In the call, I heard the thief say that they were planning to take the
earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other
end of the phone to purchase the flight ticket. |

| Lily    | Our neighboring courthouse has a very annoying rooster that crows loudly at
6am every day. My sons Robert and Patrick took the rooster to a city far, far away, so it may never
 bother us again. My sons have successfully arrived in Paris.


 UPDATE 2:
RUTH - saw thief within 10 min after rob entering car in bakery parking lot. Check security footage
EUGENE - doesnt know name but recognises him/her. Thief was withrawing money from atm on Leggett Street
RAYOMOND - After rob, thief called someone (for 1 min). thief is planning earliest flight out for tomorrow.
Thief asked accomplice to buy ticket.

QUERY 3: FIND ALL PLATES FROM 10:15 TO 10:25 IN THE BAKERY PARKING LOT
REMEMBER: THIEF LEFT BAKERY PARKING LOT AFTER CRIME AT 10:15
QUERY 3: SELECT license_plate, activity FROM bakery_security_logs WHERE
   ...> year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute >=15 AND minute <= 25;

RESULT:
+---------------+----------+
| license_plate | activity |
+---------------+----------+
| 5P2BI95       | exit     |
| 94KL13X       | exit     |
| 6P58WS2       | exit     |
| 4328GD8       | exit     |
| G412CB7       | exit     |
| L93JTIZ       | exit     |
| 322W7JE       | exit     |
| 0NTHK55       | exit     |
+---------------+----------+

QUERY 4: GET ALL ACCOUNT NUMBERS FROM ALL TRANSACTIONS FROM ATM ON LEGGETT STREET, ON THE DAY OF THE CRIME
REMEMBER: ROBBER WITHDRAW MONEY EARLIER BEFORE CRIME
QUERY 4: SELECT id, account_number, transaction_type, amount FROM atm_transactions WHERE
   ...> year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street';

RESULTS:
+-----+----------------+------------------+--------+
| id  | account_number | transaction_type | amount |
+-----+----------------+------------------+--------+
| 246 | 28500762       | withdraw         | 48     |
| 264 | 28296815       | withdraw         | 20     |
| 266 | 76054385       | withdraw         | 60     |
| 267 | 49610011       | withdraw         | 50     |
| 269 | 16153065       | withdraw         | 80     |
| 275 | 86363979       | deposit          | 10     |
| 288 | 25506511       | withdraw         | 20     |
| 313 | 81061156       | withdraw         | 30     |
| 336 | 26013199       | withdraw         | 35     |
+-----+----------------+------------------+--------+


QUERY 5: GET ALL CALLS (CALLER / RECEIVER) LESS THAN 1 MIN DURATION THAT DAY
REMEMBER: THIEF CALLED SOMEONE
QUERY 5: SELECT caller, receiver, duration FROM phone_calls WHERE
   ...> year = 2023 AND month = 7 AND day = 28 AND duration <= 60;

RESULTS:
+----------------+----------------+----------+
|     caller     |    receiver    | duration |
+----------------+----------------+----------+
| (130) 555-0289 | (996) 555-8899 | 51       |
| (499) 555-9472 | (892) 555-8872 | 36       |
| (367) 555-5533 | (375) 555-8161 | 45       |
| (609) 555-5876 | (389) 555-5198 | 60       |
| (499) 555-9472 | (717) 555-1342 | 50       |
| (286) 555-6063 | (676) 555-6554 | 43       |
| (770) 555-1861 | (725) 555-3243 | 49       |
| (031) 555-6622 | (910) 555-3251 | 38       |
| (826) 555-1652 | (066) 555-9701 | 55       |
| (338) 555-6650 | (704) 555-2131 | 54       |
+----------------+----------------+----------+


QUERY 6: GET ALL FLIGHTS RELEVANT DATA FROM THE NEXT DAY TO THE ROBBERY (29/07/2024) THAT DEPART FROM FIFTYVILLE
QUERY 6:
SELECT flights.id, origin_airport.city, destination_airports.city, flights.hour, flights.minute FROM flights
   ...> JOIN airports AS origin_airport ON origin_airport.id = flights.origin_airport_id
   ...> JOIN airports AS destination_airports ON destination_airports.id = flights.destination_airport_id
   ...> WHERE origin_airport.city = 'Fiftyville' AND year = 2023 AND month = 7 AND day = 29;

RESULTS:
+----+------------+---------------+------+--------+
| id |    city    |     city      | hour | minute |
+----+------------+---------------+------+--------+
| 18 | Fiftyville | Boston        | 16   | 0      |
| 23 | Fiftyville | San Francisco | 12   | 15     |
| 36 | Fiftyville | New York City | 8    | 20     |
| 43 | Fiftyville | Chicago       | 9    | 30     |
| 53 | Fiftyville | Tokyo         | 15   | 20     |
+----+------------+---------------+------+--------+

FINAL QUERY: JOIN ALL INFORMATION SO FAR AND RESTRICT TO PEOPLE THAT LEFT THE CITY THE NEXT DAY EARLY (FLIGHTS 36 OR 43)
FINAL QUERY:

SELECT caller.name, caller.phone_number, passengers.flight_id, receiver.name, receiver.phone_number FROM phone_calls
JOIN people AS receiver ON phone_calls.receiver = receiver.phone_number
JOIN people AS caller ON phone_calls.caller = caller.phone_number
JOIN bakery_security_logs AS bak_plate ON bak_plate.license_plate = caller.license_plate
JOIN passengers ON passengers.passport_number = caller.passport_number
JOIN bank_accounts ON caller.id = bank_accounts.person_id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE passengers.flight_id IN (36, 43) AND phone_calls.year = 2023 AND phone_calls.month = 7 AND phone_calls.day = 28 AND phone_calls.duration <= 60
AND bak_plate.year = 2023 AND bak_plate.month = 7 AND bak_plate.day = 28 AND bak_plate.hour = 10 AND bak_plate.minute >=15 AND bak_plate.minute <= 25
AND atm_transactions.year = 2023 AND atm_transactions.month = 7 AND atm_transactions.day = 28 AND atm_transactions.atm_location = 'Leggett Street';

RESULTS:
+-------+----------------+-----------+-------+----------------+
| name  |  phone_number  | flight_id | name  |  phone_number  |
+-------+----------------+-----------+-------+----------------+
| Bruce | (367) 555-5533 | 36        | Robin | (375) 555-8161 |
+-------+----------------+-----------+-------+----------------+
*/
