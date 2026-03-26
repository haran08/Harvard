-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Discover the structure of the database and the main tables
.tables;


.schema crime_scene_reports;


-- Serach for the crimes commited in the same date as the one Im looking for
SELECT COUNT(*)
FROM crime_scene_reports
WHERE year == 2024 AND
 month == 7 AND
  day == 28 AND
   street == 'Humphrey Street';


-- Search the transcripions that contein information about the thief
-- (The transcript keyword comes from the crime_scene_reports description)
SELECT name, transcript
FROM interviews
WHERE year == 2024 AND
 month == 7 AND
  day == 28 AND
   transcript LIKE '%bakery%';

-- Fiend the license_plates that are in the time describe by the 1 witness
SELECT license_plate
FROM bakery_security_logs
WHERE day == 28 AND
 month == 7 AND
  year == 2024 AND
   hour == 10 AND
    (minute > 15 OR minute <= 25) AND activity == 'exit';

-- Fiend the account_number that are in the street describe by the 2 witness
SELECT account_number
FROM atm_transactions
WHERE year == 2024 AND
 month == 7 AND
  day == 28 AND
   atm_location == 'Leggett Street' AND
    transaction_type == 'withdraw';

-- Find the caller and receiver that are speaking in the day descibre by the 3 witness
SELECT caller
FROM phone_calls
WHERE year == 2024 AND
 month == 7 AND
  day == 28 AND
   duration < 60;

-- Find id of flights that have gone in the day describe by the 3 witness
SELECT flights.id
FROM flights JOIN airports ON flights.origin_airport_id == airports.id
 WHERE year == 2024 AND
  month == 7 AND
   day == 29
ORDER BY hour
LIMIT 1;

-- Find the passport_number of the flights based on the description of the 3 witness
SELECT passport_number
FROM passengers
WHERE flight_id == (
   SELECT flights.id
   FROM flights JOIN airports ON flights.origin_airport_id == airports.id
   WHERE year == 2024 AND
          month == 7 AND
           day == 29
   ORDER BY hour
   LIMIT 1
);

-- Find suspected people id
SELECT id
FROM people
WHERE phone_number in (
   SELECT caller
   FROM phone_calls
   WHERE year == 2024 AND
   month == 7 AND
   day == 28 AND
   duration < 60)
AND passport_number IN (
   SELECT passport_number
   FROM passengers
   WHERE flight_id == (
      SELECT flights.id
      FROM flights JOIN airports ON flights.origin_airport_id == airports.id
      WHERE year == 2024 AND
          month == 7 AND
           day == 29
      ORDER BY hour
      LIMIT 1
   )
)
AND license_plate IN (
   SELECT license_plate
   FROM bakery_security_logs
   WHERE day == 28 AND
    month == 7 AND
     year == 2024 AND
     hour == 10 AND
     (minute >= 15 OR minute <= 25) AND
      activity == 'exit')

-- Find the account_number of the suspects
SELECT account_number FROM bank_accounts WHERE person_id IN (
SELECT id
FROM people
WHERE phone_number in (
   SELECT caller
   FROM phone_calls
   WHERE year == 2024 AND
   month == 7 AND
   day == 28 AND
   duration < 60)
AND passport_number IN (
   SELECT passport_number
   FROM passengers
   WHERE flight_id == (
      SELECT flights.id
      FROM flights JOIN airports ON flights.origin_airport_id == airports.id
      WHERE year == 2024 AND
          month == 7 AND
           day == 29
      ORDER BY hour
      LIMIT 1
   )
)
AND license_plate IN (
   SELECT license_plate
   FROM bakery_security_logs
   WHERE day == 28 AND
    month == 7 AND
     year == 2024 AND
     hour == 10 AND
     (minute >= 15 OR minute <= 25) AND
      activity == 'exit')
   )
AND account_number IN (
   SELECT account_number
   FROM atm_transactions
   WHERE year == 2024 AND
   month == 7 AND
   day == 28 AND
      atm_location == 'Leggett Street' AND
       transaction_type == 'withdraw'
);

-- Find informations about the suspects
SELECT name, phone_number, id
FROM people
WHERE id IN (
   SELECT person_id FROM bank_accounts WHERE person_id IN (
   SELECT id
   FROM people
   WHERE phone_number in (
      SELECT caller
      FROM phone_calls
      WHERE year == 2024 AND
      month == 7 AND
      day == 28 AND
      duration < 60)
   AND passport_number IN (
      SELECT passport_number
      FROM passengers
      WHERE flight_id == (
         SELECT flights.id
         FROM flights JOIN airports ON flights.origin_airport_id == airports.id
         WHERE year == 2024 AND
             month == 7 AND
              day == 29
         ORDER BY hour
         LIMIT 1
      )
   )
   AND license_plate IN (
      SELECT license_plate
      FROM bakery_security_logs
      WHERE day == 28 AND
       month == 7 AND
        year == 2024 AND
        hour == 10 AND
        (minute >= 15 OR minute <= 25) AND
         activity == 'exit')
      )
   AND account_number IN (
      SELECT account_number
      FROM atm_transactions
      WHERE year == 2024 AND
      month == 7 AND
      day == 28 AND
         atm_location == 'Leggett Street' AND
          transaction_type == 'withdraw'
   )
);

-- Find the all the calls of the suspects
SELECT caller, receiver, duration
   FROM phone_calls
   WHERE year == 2024 AND
   month == 7 AND
   day == 28 AND
   duration < 60 AND (
      caller == '(286) 555-6063' OR
      caller == '(367) 555-5533'
   );


-- Find the name and id of the thief and his accomplice
SELECT id, name, phone_number
FROM people
WHERE phone_number == '(367) 555-5533' OR
   phone_number == '(375) 555-8161';

-- Find place where thief has gone

SELECT city
FROM airports
WHERE id == (
         SELECT flights.destination_airport_id
         FROM flights JOIN airports ON flights.origin_airport_id == airports.id
         WHERE year == 2024 AND
             month == 7 AND
            day == 29
         ORDER BY hour
         LIMIT 1
);
