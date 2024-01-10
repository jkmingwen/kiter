/*
 * commons.cpp
 *
 *  Created on: 1 févr. 2011
 *      Author: toky
 */

#include <commons/commons.h>
#include <commons/verbose.h>
#include <tuple>

namespace commons {

    std::vector<std::string> parseCsvLine(const std::string& line) {
        std::vector<std::string> cells;
        std::string cell;
        std::istringstream lineStream(line);
        bool inQuotes = false;

        for(char c : line) {
            switch(c) {
                case ',':
                    if(inQuotes) {
                        cell += c;
                    } else {
                        cells.push_back(cell);
                        cell = "";
                    }
                    break;

                case '\"':
                    inQuotes = !inQuotes;
                    break;

                default:
                    cell += c;
                    break;
            }
        }
        // Add the last cell
        cells.push_back(cell);
        return cells;
    }



    TIME_UNIT roundIt(TIME_UNIT val,TIME_UNIT p) {
  std::stringstream s;
  s<<std::setprecision((int)p)<<std::setiosflags(std::ios_base::fixed)<<val;
   s>>val;
   return val;
}

template<>
std::string toString< std::vector<TOKEN_UNIT> >(const std::vector<TOKEN_UNIT>& v)
{
        return commons::join(v.begin(),v.end(),std::string(","));
}
template<>
std::string toString(const std::vector<std::string>& t) {

        std::stringstream s;
        s << "{";
        bool first = true;
        for (auto myt : t) {
            if (!first) s << ",";
            s << "\"" << commons::toString(myt) << "\"";
            first = false;
        }
        s << "}";
        return s.str();
    }

template<>
    std::string toString(const std::set<long  int, std::less<long  int>, std::allocator<long  int> >& t) {
	 std::stringstream s;
	 for (auto myt : t) {
		 s << commons::toString(myt) + " ";
	 }
	 return s.str();
}
template<>
    std::string toString(const std::set<long unsigned int, std::less<long unsigned int>, std::allocator<long unsigned int> >& t) {
	 std::stringstream s;
	 for (auto myt : t) {
		 s << commons::toString(myt) + " ";
	 }
	 return s.str();
}

template<>
std::string toString< std::vector < std::tuple<ARRAY_INDEX, ARRAY_INDEX, ARRAY_INDEX> > >(const std::vector < std::tuple<ARRAY_INDEX, ARRAY_INDEX, ARRAY_INDEX> >& v)
{
	 std::stringstream s;
	for (auto myt : v) {
		s << commons::toString(myt) + " ";
	}
	return s.str();
}



template<>
std::string toString< std::tuple<ARRAY_INDEX, ARRAY_INDEX, ARRAY_INDEX> >(const std::tuple<ARRAY_INDEX, ARRAY_INDEX, ARRAY_INDEX>& v)
{
        return "<" + commons::toString(std::get<0>(v)) +  "," +  commons::toString(std::get<1>(v)) +  "," +  commons::toString(std::get<2>(v)) +  "," + ">";
}
template<>
std::string toString< std::pair<TIME_UNIT , ARRAY_INDEX> >(const std::pair<TIME_UNIT , ARRAY_INDEX>& v)
{
        return "<" + commons::toString(std::get<0>(v)) +  "," +  commons::toString(std::get<1>(v)) +  "," + ">";
}

template<>
std::string toString< std::pair<unsigned long , unsigned long> >(const std::pair<unsigned long , unsigned long>& v)
{
        return "<" + commons::toString(std::get<0>(v)) +  "," +  commons::toString(std::get<1>(v)) +  "," + ">";
}


template<>
std::string toString< std::vector<long double> >(const std::vector<long double>& v)
{
        return  "{" + commons::join(v.begin(),v.end(),std::string(","))+ "}";
}
template<>
std::string toString< std::vector<unsigned int> >(const std::vector<unsigned int>& v)
{
        return  "{" + commons::join(v.begin(),v.end(),std::string(","))+ "}";
}
template<>
std::string toString< std::vector<int> >(const std::vector<int>& v)
{
        return "{" + commons::join(v.begin(),v.end(),std::string(",")) + "}";
}

template<>
	char *fromString<char*>(const std::string&)
{
		VERBOSE_BAD_PROGRAMMING(); // niak niak niak ....
		return 0;
}
template<>
	const char *fromString<const char*>(const std::string& str)
{
      return str.c_str();
}

char * string2char(const std::string& str) {
	char * ret;
	ret = new char[str.length() + 1];
	strcpy(ret, str.c_str());
	return ret;
}

bool is_readable( const std::string & file )
{
    std::ifstream fichier( file.c_str() );
    return !fichier.fail();
}

 std::string getEnv(std::string name) {
    char const* tmp = getenv( name.c_str() );
    if ( tmp == NULL ) {
        return "";
    } else {
        std::string res (tmp);
        return res;
    }
}

 std::string latexize(std::string text) {
     std::string nouveau = text;
     size_t position = nouveau.find( "_" ); // find first

     while ( position != std::string::npos )
        {
         nouveau.replace( position, 1, "\\_" );
         position = nouveau.find( "_", position + 2);
        }
     return nouveau;
 }



 std::vector<std::string> splitSDF3List(const std::string &s) {
     std::vector<std::string> sub,ssub;
     std::vector<std::string> elems;
     sub = split <std::string> (s, ',');
     for (std::vector<std::string>::iterator it = sub.begin() ; it != sub.end() ; it++) {
         ssub = split<std::string> (*it, '*');
         if (ssub.size() == 1) {
             elems.push_back((ssub.at(0)));
         } else if (ssub.size() == 2) {
             for (int i = 0 ; i < commons::fromString<int>(ssub.at(0)) ; i++) {elems.push_back((ssub.at(1)));}
         } else {
             VERBOSE_FAILURE();
         }
     }
     return elems;
 }

 int fibo (int index) {
     int fib_n  = 0;
     int fib_np = 1;
     for (int i = 0; i < index ; i++){
          int swap = fib_np;
          fib_n = fib_np;
          fib_np = swap + fib_np;
     }
     return fib_n;
 }

 const std::string runCmd( const std::string com , TIME_UNIT  max_time ) {

	 std::string sendme = com;
	 if (max_time > 0)
		 sendme = "timeout " + commons::toString<unsigned int>((unsigned int)max_time) + " " + sendme + " 2>/dev/null";

	 VERBOSE_INFO("launch command : " << sendme );

	 FILE * fp = popen (sendme.c_str(),"r");
	 char buffer[1024];
	 std::string res;
	 size_t readcount;
	 do {
		 readcount = fread(buffer,sizeof(char),sizeof(buffer),fp);
		 res += std::string(buffer).substr (0,readcount);
	 } while (readcount) ;

	 pclose(fp);

	 return res;

 }



}
