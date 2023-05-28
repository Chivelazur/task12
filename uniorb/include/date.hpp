#pragma once

#include <string>
#include <vector>

namespace uniorb {

/* 
    Это класс, который я писал для диссертации.
    Позволяет ловко работать со временем без потерей в точности.
*/
class date {
public:
    date();
    date(const date & Date);
    date(date && Date);
    date(const std::string & Date);
    date(std::string && Date);
    date(double Mjd);
    date(long Mjdn, double Fraction);
    date(int Year, int Month, int Day, int HH, int MM, double SS);
    date & operator=(const date & Date);
    date & operator=(date && Date);

    bool operator>(const date & Date) const;
    bool operator<(const date & Date) const;

    void set_fraction(double Fraction);
    void set_mjdn(long Mjdn);
    void set_jd(double Jd); 
    void set_mjd(double Mjd);
    void set_date(const date & Date);
    void set_date(const std::string & Date);
    void set_date(std::string && Date);
    void set_date(double Mjd);
    void set_date(long Mjdn, double Fraction);
    void set_date(int Year, int Month, int Day, int HH, int MM, double SS);
    void add(double Seconds);
    void add(const date & Date);

    double get_secs2000() const;
    double get_days2000() const;
    long get_mjdn() const;
    double get_fraction() const;
    double get_mjd() const;
    double get_jd() const;
    long get_year() const;
    long get_doy() const;
    std::string get_date(unsigned char Precision = 0) const;
    std::string get_date_2(unsigned char Precision) const;
    std::string get_time(unsigned char Precision = 0) const;
    std::string get_month(int Month) const;
    
    static date now();
private:
    // Modified Julian date of the beginning of the day (MJDN).
    long _mjdn = 0;
    // Seconds past since the beginning of the day.
    double _fraction = 0;
    static inline std::vector<double> _split(std::string && Date);
};

}