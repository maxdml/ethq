/*
 * Copyright (C) Internet Systems Consortium, Inc. ("ISC")
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * See the COPYRIGHT file distributed with this work for additional
 * information regarding copyright ownership.
 */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <regex>

//
// abstrat base class for stats string parsers, includes a static
// registry that maps from NIC driver names to parsers
//

class StringsetParser {

public:
	typedef StringsetParser* ptr_t;
	typedef std::map<std::string, ptr_t> parsermap_t;
	typedef std::vector<std::string> driverlist_t;

private:
	static parsermap_t parsers;

protected:
	void save(const std::string& driver);
	void save(const driverlist_t& drivers);

public:
	StringsetParser(const driverlist_t& drivers);
	virtual bool match(const std::string& key, size_t value, size_t& queue, bool& rx, bool& bytes) = 0;

public:
	static ptr_t find(const std::string& driver);
};

//
// concrete class that knows how to extract the three fields
// from groups in a regex
//
// the `order` table specifies the order in which the three
// fields (direction, queue number, metric type) appear as
// groups within the regex
//
// NB: match on "direction" requires an exact match for "rx"
//     match on "type" requires an exact match for "bytes"
//
class RegexParser : public StringsetParser {

public:
	typedef std::array<int, 3> order_t;

protected:
	order_t		order;
	std::regex      re;
	std::smatch     ma;
	std::string     ms(size_t n);

public:
	RegexParser(const driverlist_t& drivers,
		    const std::string& match,
		    const order_t& order = { 1, 2, 3});

	virtual bool match(const std::string& key, size_t value, size_t& queue, bool& rx, bool& bytes);
};
