# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2025, Benoit BLANCHON
# MIT License

add_executable(JsonArrayTests
	add.cpp
	clear.cpp
	compare.cpp
	copyArray.cpp
	equals.cpp
	isNull.cpp
	iterator.cpp
	nesting.cpp
	remove.cpp
	size.cpp
	subscript.cpp
	unbound.cpp
)

add_test(JsonArray JsonArrayTests)

set_tests_properties(JsonArray
	PROPERTIES
		LABELS "Catch"
)
