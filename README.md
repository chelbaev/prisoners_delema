запуск самого cmake:

cmake -S. bin  
cmake --build bin

запуск main:

./bin/prisoners WoDT/WDT *count_of_step* *>=3 name_of_strategy*

WDT - с детализацией

WoDT - без детализации

name_of_strategy - always_coop, always_betr, alternative_coop, and_coop, or_coop, random

запуск гугл тестов:
./bin/test/test_app
