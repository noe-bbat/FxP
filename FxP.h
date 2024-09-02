//
// Created by noe on 16/07/24.
//
#include <iostream>
#include <string>
#include <cmath>
#include <stdint.h>
#include <stdlib.h>
#include <math.h> 

#ifndef FXP_H
#define FXP_H

enum wrap_mode { WRAP_AROUND, SAT_ZERO, SAT_MINMAX };
enum round_mode {TRUNC, ROUND_INF, ROUND_ZERO, ROUND_ODD}; 

template<int m, int l, bool s, wrap_mode o = SAT_MINMAX,round_mode r = ROUND_ZERO>
class FxP{
private:
    int w ;
    int _m;//msb
    int _l;//lsb
    int _s;//signe
    wrap_mode _o;//manageur d'overflow
    round_mode _r;//manageur d'arrondis 

public:
    //constructeurs 
    template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2> friend class FxP;
	inline FxP():w(0),_m(m),_l(l),_s(s),_o(o),_r(r){};
    
    template<int m2, int l2, bool s2 ,wrap_mode o2,round_mode r2> friend class FxP;
    inline FxP(double d):_m(m),_l(l),_s(s),_o(o),_r(r){
        switch(s){
			case(0):
			if (r == TRUNC){w = (unsigned) floor(std::ldexp(d,-l));}
			else if (r == ROUND_INF){w = (unsigned) ceil(std::ldexp(d,-l));}
			else {w = (unsigned) round(std::ldexp(d,-l));}
			if ((w >= std::ldexp(1,m-_l))||(w < 0)) { overflow_manager(o,_s); }
			break;

			case(1):
			if (r == TRUNC){w = (unsigned) floor(std::ldexp(d,-l));}
			else if (r == ROUND_INF){w = (signed) ceil(std::ldexp(d,-l));}
			else {w = (signed) round(std::ldexp(d,-l));}
			if ((w <= std::ldexp(-1,m-1-_l))||(w >= std::ldexp(1,m-_l-1))) { overflow_manager(o,_s);}
			break;
		}
		//if (std::ldexp(w,l) > d){w = w - std::ldexp(1,l);}
		rounding_manager(r, d);
    };
    
    inline void overflow_manager(wrap_mode O, bool S){
		switch(s){
			case(0):
			switch(o){
				case(WRAP_AROUND):
				while((w >= std::ldexp(1,m-l))||(w < 0)){
					if(w >= std::ldexp(1,m-l)){w = w - std::ldexp(1,m-l);}
					if(w < 0){w = w + std::ldexp(1,m-l);}
				}
				//w = w - std::ldexp(1,m-l);
				break;
				case(SAT_MINMAX):
				if (w <= 0) {w = 0;} else {w = std::ldexp(1,m-l) - std::ldexp(1,l);}
				break;
				case(SAT_ZERO):
				w = 0;
				break;
			}
			break;

			case(1):
			switch(o){
				case(WRAP_AROUND):
				while((w >= std::ldexp(1,m-l-1))||(w < std::ldexp(-1,m-l-1))){
					if(w >= std::ldexp(1,m-l-1)){w = w - std::ldexp(1,m-l);}
					if(w < std::ldexp(-1,m-l-1)){w = w + std::ldexp(1,m-l);}
				}
				break;
				case(SAT_MINMAX):
				if (w <= 0) {w = std::ldexp(-1,m-l-1);} else {w = std::ldexp(1,m-l-1) - std::ldexp(1,l);}
				break;
				case(SAT_ZERO):
				w = 0;
				break;
			}
			break;
		}
	};
	
	inline void rounding_manager(round_mode R, double d){
		
		switch(R){
			case(TRUNC):
			//w = floor(std::ldexp(w, -D)); 
			break;
			case(ROUND_INF):
			if ((o == WRAP_AROUND)&&(std::ldexp(w,l) < d)){w = w - 1;}
			//std::cerr << " zqbhfkebh " << d  << w << std::endl;
			break;
			case(ROUND_ZERO):
			if ((o != WRAP_AROUND)&&(this->getFxP() < d)&&(std::ldexp(w,l)) < 0){w = w + std::ldexp(1,_l);}
			
			if ((o != WRAP_AROUND)&&(this->getFxP() > d)&&(std::ldexp(w,l)) > 0){w = w - std::ldexp(1,l);}
			break;
			case(ROUND_ODD):
			std::bitset<m-l+1> bits(w);
			if ((std::ldexp(w,l) != d)&&(bits[0] == 0)){
				w = w + std::ldexp(1,l);}
			break;
		}
	};
	
	int minL(double d, bool S){
		double ma, M;
		int ml;
		double D = d;
		if(D < 0){ma = ceil(log2(-D));}
		else {ma = floor(log2(D))+S;}
		M = round(std::ldexp(D,m-l-ma));
		if (M == std::ldexp(1,m-l-S+1)) {++ma;}
		else if (M == std::ldexp(-1,m-l-2)) {--ma;}
		ml = ma + 1 - (m-l);
		return ml;
	};
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2> 
	inline FxP(const FxP<m2, l2, s2, o2, r2>& fx):_m(m),_l(l),_s(s||s2){
		if ((m >= m2)&&(l == l2)&&(s == s2)){
			w = fx.getMantisse();
				
		} else {
			if (r == TRUNC){w= floor((std::ldexp(fx.getFxP(),-l)));	}
			else{w= round((std::ldexp(fx.getFxP(),-l)));}	
			}
			rounding_manager(r,w);
			
			switch(s&&s2){
			case(0):
			if ((w >= std::ldexp(1,m-_l))||(w < 0)) { overflow_manager(o,_s); }
			break;

			case(1):
			if ((w <= std::ldexp(-1,m-_l))||(w >= std::ldexp(1,m-_l-1))) { overflow_manager(o,_s);}
			break;
		}	
	};
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	FxP& operator+=(const FxP<m2,l2,s2,o2,r2> &y){

      *this = this->getFxP() + y.getFxP();
      
      return *this;
	}
	
	inline FxP& operator+=(const double d){

      *this = this->getFxP() + d;
      
      return *this;
	}
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	inline FxP& operator-=(const FxP<m2,l2,s2,o2,r2> &y){

      *this = this->getFxP() - y.getFxP();
      return *this;
	}
	
	inline FxP& operator-=(const double d){

      *this = this->getFxP() - d;
      return *this;
	}
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	FxP& operator*=(const FxP<m2,l2,s2,o2,r2> &y){

      *this = this->getFxP() * y.getFxP();
      return *this;
	}
	
	FxP& operator*=(const double d){

      *this = this->getFxP() * d;
      return *this;
	}
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	FxP& operator/=(const FxP<m2,l2,s2,o2,r2> &y){

      *this = this->getFxP() / y.getFxP();
      return *this;
	}
	
	FxP& operator/=(const double d){

      *this = this->getFxP() / d;
      return *this;
	}
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	FxP& operator&=(const FxP<m2,l2,s2,o2,r2> &y){

      *this = this->getFxP() & y.getFxP();
      return *this;
	}
	
	FxP& operator&=(const double d){

      *this = this->getFxP() & d;
      return *this;
	}
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	FxP& operator|=(const FxP<m2,l2,s2,o2,r2> &y){

      *this = this->getFxP() | y.getFxP();
      return *this;
	}
	
	FxP& operator|=(const double d){

      *this = this->getFxP() | d;
      return *this;
	}
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	FxP& operator^=(const FxP<m2,l2,s2,o2,r2> &y){

      *this = this->getFxP() ^ y.getFxP();
      return *this;
	}
	
	FxP& operator^=(const double d){

      *this = this->getFxP() ^ d;
      return *this;
	}
	
    //méthodes
    inline int getMantisse() const {return w;};
    inline double getFxP() const{return (std::ldexp(w,_l));};
    
    
    
    
  template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
  struct fp {
    enum {
      m_plus=m+1, //sert aussi pour la soustraction
      l_plus=l, //sert aussi pour la soustraction
      s_plus=s||s2, //sert aussi pour la multiplication
      s_moins = 1,
      m_mult=m+m2+1,
      l_mult=l+l2,
      m_div=m-l2
    };
    typedef FxP<m_plus, l_plus,s_plus,o,r> plus;
    typedef FxP<m_plus, l_plus,s_moins,o,r> minus;
    typedef FxP<m_mult, l_mult,s_plus,o,r> mult;
    typedef FxP<m_div, l_plus,s_plus,o,r> div;
    typedef FxP<m_plus, l_plus,s_plus,o,r> logic;
   };
    
	
	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	typename fp<m2,l2,s2,o2,r2>::plus operator +(const FxP<m2,l2,s2,o2,r2> &y) const {
    if (m2 != m){
		std::cerr << "Erreur: MSB différents." << std::endl;
		exit(0);
	} else if (l2 != l) {
		std::cerr << "Erreur: LSB différents." << std::endl;
		exit(0);
	} else{
		FxP<m+1,l,s||s2,o,r> res = this->getFxP() + y.getFxP();
		return res;
	}
  }
  
  FxP operator +(double d) const {
		int temp =  this->getMantisse() + std::ldexp(d,-l) ;
        FxP<m+1, l, s,o,r> res = std::ldexp(temp,l);
        return res;
    }
  
  
   
    
   
    
  
  
  template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	typename fp<m2,l2,s2,o2,r2>::minus operator -(const FxP<m2,l2,s2,o2,r2> &y) const {
    if (m2 != m){
		std::cerr << "Erreur: MSB différents." << std::endl;
		exit(0);
	} else if (l2 != l) {
		std::cerr << "Erreur: LSB différents." << std::endl;
		exit(0);
	}
	
	FxP<m+1,l,1,o,r> res =(this->getFxP() - y.getFxP());
	return res;
	
  }
  
  FxP operator -(double d) const {
		int temp =  this->getMantisse() - std::ldexp(d,-l) ;
        FxP<m+1, l, 1,o,r> res = std::ldexp(temp,l);
        return res;
    }
  
  template<int m2, int l2, bool s2,wrap_mode o2,round_mode r2>
	typename fp<m2,l2,s2,o2,r2>::mult operator *(const FxP<m2,l2,s2,o2,r2> &y) const {
    FxP<m+m2+1,l+l2,s||s2,o,r> res = std::ldexp(w * y.getMantisse(),l+l2);
	return res;
  }
  
  FxP operator *(double d) const {
		FxP<m+1, l, 1,o,r> temp = d;
		double dd = this->getFxP() * temp.getFxP();
		FxP<2*m, 2*l, 0,o,r> res0 = this->getFxP() * temp.getFxP();
		FxP<2*m, 2*l, 1,o,r> res1 = this->getFxP() * temp.getFxP();
		if (dd < 0 ) {return res1;}
		else {return res0;}
        //FxP<2*m, 2*l, st,o,r> res = this->getFxP() * temp.getFxP();
        
        //return res;
    }

	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	typename fp<m2,l2,s2,o2,r>::div operator /(const FxP<m2,l2,s2,o2,r> &y) const {
    FxP<m-l2,l,s||s2,o,r> res = this->getFxP() / y.getFxP();
	return res;
  }
  
  FxP operator /(double d) const {
		FxP<m, l, s,o,r> temp = d;
        FxP<m, l, 0,o,r> res = this->getFxP() / temp.getFxP();
        return res;
    }
  

	template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	typename fp<m2,l2,s2,o2,r2>::logic operator &(const FxP<m2,l2,s2,o2,r2> &y) const {
	int temp = round(std::ldexp(y.getFxP(),-l));
    FxP<m+1,l,s||s2,o,r> res = std::ldexp(this->getMantisse() & temp,l);
	return res;
  }
  
  FxP operator &(double d) const {
		FxP<m, l, s,o,r> temp = d;
		int temp2 = round(std::ldexp(temp.getFxP(),-l));
        FxP<m+1, l, s,o,r> res = std::ldexp(this->getMantisse() & temp2,l);
        return res;
    }
    
   template<int m2, int l2, bool s2, wrap_mode o2,round_mode r2>
	typename fp<m2,l2,s2,o2,r2>::logic operator |(const FxP<m2,l2,s2,o2,r2> &y) const {
	int temp = round(std::ldexp(y.getFxP(),-l));
    FxP<m+1,l,s||s2,o,r> res = std::ldexp(this->getMantisse() | temp,l);
	return res;
  }
  
  FxP operator |(double d) const {
		FxP<m, l, s,o,r> temp = d;
		int temp2 = round(std::ldexp(temp.getFxP(),-l));
        FxP<m+1, l, s,o,r> res = std::ldexp(this->getMantisse() | temp2,l);
        return res;
    }
    
    template<int m2, int l2, bool s2, wrap_mode o2>
	typename fp<m2,l2,s2,o2,r>::logic operator ^(const FxP<m2,l2,s2,o2,r> &y) const {
	int temp = round(std::ldexp(y.getFxP(),-l));
    FxP<m+1,l,s||s2,o,r> res = std::ldexp(this->getMantisse() ^ temp,l);
	return res;
  }
  
  FxP operator ^(double d) const {
		FxP<m, l, s,o,r> temp = d;
		int temp2 = round(std::ldexp(temp.getFxP(),-l));
        FxP<m+1, l, s,o,r> res = std::ldexp(this->getMantisse() ^ temp2,l);
        return res;
    }


};
	
	
	template<int m, int l, bool s, wrap_mode o,round_mode r>
    std::ostream& operator<<(std::ostream& os,const FxP<m,l,s,o,r> &x){
    os << x.getFxP() ;
    return os;
	}

#endif //FXP_H
