#include "../include/date.hpp"

#include <sstream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <unordered_map>

namespace uniorb {

// Creates new Date with the MJDN = 0, Fraction = 0
date::date() : _mjdn(0), _fraction(0) {}

// Copy constructor for date.
date::date(const date & Date) {
    set_date(Date);
}

date::date(date && Date) {
	_mjdn = std::move(Date._mjdn);
	_fraction = std::move(Date._fraction);
}
// Creates new Date and sets parameters from the input string Date.
// Input:
//  1) Date - date in the format YYYY-MM-DDTHH:MM:SS.SSS
// Output:
//  -
// Exceptions:
//  1) WRONG_INPUT_DATA
date::date(const std::string & Date) {
    set_date(Date);
}

date::date(std::string && Date) {
    set_date(std::move(Date));
}

date::date(double Mjd) {
    set_date(Mjd);
}

date::date(long Mjdn, double Fraction) {
    set_date(Mjdn, Fraction);
}

date::date(int Year, int Month, int Day, int HH, int MM, double SS) {
    set_date(Year, Month, Day, HH, MM, SS);
}

date & date::operator=(const date & Date) {
	if (this != &Date) {
		set_date(Date._mjdn, Date._fraction);
	}
	return *this;
}

date & date::operator=(date && Date) {
	if (this != &Date) {
		_mjdn = std::move(Date._mjdn);
		_fraction = std::move(Date._fraction);
	}
	return *this;
}

bool date::operator>(const date & Date) const {
	if (_mjdn == Date._mjdn) {
		return _fraction > Date._fraction;
	}
	return _mjdn > Date._mjdn;
}

bool date::operator<(const date & Date) const {
	if (_mjdn == Date._mjdn) {
		return _fraction < Date._fraction;
	}
	return _mjdn < Date._mjdn;
}

void date::set_fraction(double Fraction) {
	// In case if Fraction > 86400
    _fraction = 0;
    add(Fraction);
}

void date::set_mjdn(long Mjdn) {
	_mjdn = Mjdn;
}

void date::set_jd(double Jd) {
	set_mjd(Jd - 2400000.5);
} 

void date::set_mjd(double Mjd) {
	_mjdn = (long)(Mjd);
	_fraction = Mjd - _mjdn;
}

void date::set_date(const date & Date) {
    _mjdn = Date.get_mjdn();
    _fraction = Date.get_fraction();
}

void date::set_date(const std::string & Date) {
    try {
        // Need to convert string to double
        // arr = {YYYY = 0, MM = 1, DD = 2, HH = 3, MM = 4, SS = 5}
	    auto arr = _split(std::string(Date));
        // Check the size of the array
        if (arr.size() != 6) throw 0;
        // Continue calculations
		set_date((int) arr[0], (int) arr[1], (int) arr[2], (int) arr[3], (int) arr[4], arr[5]);
    }
    catch (...) {
        return;
    }
}

void date::set_date(std::string && Date) {
    try {
	    auto arr = _split(std::move(Date));
        if (arr.size() != 6) throw 0;
		set_date((int) arr[0], (int) arr[1], (int) arr[2], (int) arr[3], (int) arr[4], arr[5]);
    }
    catch (...) {
        return;
    }
}

void date::set_date(double Mjd) {
    _mjdn = (long) (Mjd);
	_fraction = (Mjd - _mjdn) * 86400.;
}

void date::set_date(long Mjdn, double Fraction) {
    _mjdn = Mjdn;
    // In case if _fraction > 86400
    _fraction = 0;
    add(Fraction);
}

void date::set_date(int Year, int Month, int Day, int HH, int MM, double SS) {
	long a = (long)(14 - Month)/12;
	long y = (long) (Year + 4800 - a);
	long m = (long)(Month + 12*a - 3);
	_mjdn = (long)(Day) + (153 * m + 2)/5 + 365*y + y/4 - y/100 + y/400 - 2432046;
	// In case if _fraction > 86400
    _fraction = 0;
    add(SS + MM * 60 + HH * 3600);
}

void date::add(double Seconds) {
	_fraction += Seconds;
	if (_fraction > 86400. || _fraction < 0.) {
		long z = (long) floor(_fraction / 86400.);
		_mjdn += z;
		_fraction -= z * 86400.;
	}
}

void date::add(const date & Date) {
    _mjdn += Date._mjdn;
    add(Date._fraction);
}

double date::get_secs2000() const {
	return _fraction + 86400 * (_mjdn - 51544.5);
}

double date::get_days2000() const {
    return _fraction / 86400. + (_mjdn - 51544.5);;
}

long date::get_mjdn() const {
    return _mjdn;
}

double date::get_fraction() const {
    return _fraction;
}

double date::get_mjd() const {
    return _fraction / 86400. + _mjdn;
}

double date::get_jd() const {
    return get_mjd() + 2400000.5;
}

long date::get_year() const {
	long a = _mjdn + 2432045;
	long b = (long)(4*a + 3) / 146097;
	long c = (long)(a - ( (146097*b) / 4));
	long d = (4*c + 3) / 1461;
	long e = c - ( (1461*d) / 4);
	long m = (5*e + 2) / 153;
	return 100 * b + d - 4800 + (m/10);
}

long date::get_doy() const {
	long a = _mjdn + 2432045;
	long b = (long)(4*a + 3) / 146097;
	long c = (long)(a - ( (146097*b) / 4));
	long d = (4*c + 3) / 1461;
	long e = c - ( (1461*d) / 4);
	long m = (5*e + 2) / 153;
	
	long Year = 100 * b + d - 4800 + (m/10);
	long y = (int) (Year + 4800 - 1);
	long MJDN_yearbegin = 306 + 365*y + y/4 - y/100 + y/400 - 2432046;
	return _mjdn - MJDN_yearbegin;
}

std::string date::get_date(unsigned char Precision) const {
    long a = _mjdn + 2432045;
	long b = (long)(4*a + 3) / 146097;
	long c = (long)(a - ( (146097*b) / 4));
	long d = (4*c + 3) / 1461;
	long e = c - ( (1461*d) / 4);
	long m = (5*e + 2) / 153;
	long Year = 100 * b + d - 4800 + (m/10);
	long Month = m + 3 - 12 * (m/10);
	long Day = e + 1 - (153*m + 2) / 5;
	
	std::stringstream str;

	str << std::to_string(Year) + "-";
			
	if (Month < 10) {
        str << '0';
    }
    str << std::to_string(Month) +  "-";
	
	if (Day < 10) {
        str << '0';
    }
    str << std::to_string(Day);
	str << "T" + get_time(Precision);
	
	return str.str();
}

std::string date::get_date_2(unsigned char Precision) const {
	// 1 Jun 2027 03:15:49.225
    long a = _mjdn + 2432045;
	long b = (long)(4*a + 3) / 146097;
	long c = (long)(a - ( (146097*b) / 4));
	long d = (4*c + 3) / 1461;
	long e = c - ( (1461*d) / 4);
	long m = (5*e + 2) / 153;
	long Year = 100 * b + d - 4800 + (m/10);
	long Month = m + 3 - 12 * (m/10);
	long Day = e + 1 - (153*m + 2) / 5;
	
	std::stringstream str;
	if (Day < 10) {
        str << '0';
    }
	str << std::to_string(Day) + " ";

	str << get_month(Month) +  " ";

	str << std::to_string(Year);
    
	str << " " + get_time(Precision);
	
	return str.str();
}

std::string date::get_time(unsigned char Precision) const {
	std::stringstream str;
	int h = (int) (_fraction / 3600.);
	int m = (int) ((_fraction - h * 3600) / 60.);
	double s = _fraction - h * 3600 - m * 60;
	
	// Prepare accuracy
	double acc = 1.;
	for (int i = 0; i < Precision; i++){
		acc *= 0.1;
	}

	// Check nearness to 60 and 0
	if (std::abs(s - 60) < acc){
		s = 0.;
		m++;
		if (m == 60){
			m = 0;
			h++;
		}
	}
	else if (abs(s) < acc){
		s = 0.;
	}

	// Hours
	if (h < 10){
		str << '0';
	}
	str << std::to_string(h) + ":";
	
	// Minutes
	if (m < 10){
		str << '0';
	}
	str << std::to_string(m) + ":";		
	
	// Seconds
    if (s < 10){
        str << '0';
		//--Precision;
    }
	str << std::fixed;
    str.precision(Precision);
    str << s;
	return str.str();
}

std::string date::get_month(int Month) const {
    // Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
	static auto Months = std::unordered_map<int, std::string> ();
    Months.insert_or_assign(1, "Jan");
    Months.insert_or_assign(2, "Feb");
    Months.insert_or_assign(3, "Mar");
    Months.insert_or_assign(4, "Apr");
    Months.insert_or_assign(5, "May");
    Months.insert_or_assign(6, "Jun");
    Months.insert_or_assign(7, "Jul");
    Months.insert_or_assign(8, "Aug");
    Months.insert_or_assign(9, "Sep");
    Months.insert_or_assign(10, "Oct");
    Months.insert_or_assign(11, "Nov");
    Months.insert_or_assign(12, "Dec");
    return Months.at(Month);
}

date date::now() {
	using namespace std::chrono;
	auto p1 = system_clock::now();
	double Epoch = 40587; // <- MJD. JD = 2440587.50000 for 1970-01-01 00:00:00
	double SecsPastEpoch = (double) (p1.time_since_epoch().count() * system_clock::period::num / system_clock::period::den);
	return date(40587, SecsPastEpoch);
}

// YYYY-MM-DDTHH:MM:SS.SSS
std::vector<double> date::_split(std::string && Date) {
    using namespace std;
    // Replace '-', 'T', ':' by ' '
    replace(Date.begin(), Date.end(), '-', ' ');
    replace(Date.begin(), Date.end(), 'T', ' ');
    replace(Date.begin(), Date.end(), ':', ' ');
    
    // Push strings to the vector
    vector<double> res;
    stringstream ss(Date);
    double temp;
    while (ss >> temp) {
        res.push_back(temp);
    }
    return res;
}

}