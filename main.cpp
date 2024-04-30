#include<iostream>
#include<vector>
#include<fstream>
#include<SFML/Graphics.hpp>
#include <utility>

using namespace sf;
using namespace std;

class LONGNUM {
 public:
  const int sz = 300;
  vector<int> num;

  bool operator>(const LONGNUM& other) const {
      if (other.num.size() != num.size()) {
          return num.size() > other.num.size();
      }
      for (int i = 0; i < num.size(); ++i) {
          if (num[i] != other.num[i]) {
              return num[i] > other.num[i];
          }
      }
      return false;
  }

  bool operator!=(const LONGNUM& other) const {
      if (other.num.size() != num.size()) {
          return true;
      }
      for (int i = 0; i < num.size(); ++i) {
          if (num[i] != other.num[i]) {
              return true;
          }
      }
      return false;
  }

  void del(const int x) {
      int cr = 0;
      for (int &t : num) {
          int cur = t + cr * 10;
          t = cur / x;
          cr = cur % x;
      }
      reverse(num.begin(), num.end());
      while (num.size() > 1 && num.back() == 0) {
          num.pop_back();
      }
      reverse(num.begin(), num.end());
  }

  LONGNUM& operator=(const LONGNUM& other) {
      num.resize(other.num.size());
      for (int i = 0; i < (int)other.num.size(); ++i) {
          num[i] = other.num[i];
      }
      return *this;
  }

  void operator+=(const LONGNUM& other) {
      *this = *this + other;
  }

  LONGNUM operator+(const LONGNUM& other) {

      int pointer_first = (int)num.size();
      int pointer_second = (int)other.num.size();
      LONGNUM res;
      pointer_first--, pointer_second--;
      int last = 0;
      while (pointer_first >= 0 && pointer_second >= 0) {
          res.num.push_back((num[pointer_first] + other.num[pointer_second] + last) % 10);
          if (num[pointer_first] + other.num[pointer_second] + last >= 10) {
              last = 1;
          } else {
              last = 0;
          }
          pointer_first--, pointer_second--;
      }
      while (pointer_first >= 0) {
          res.num.push_back((num[pointer_first] + last) % 10);
          if (num[pointer_first] + last >= 10) {
              last = 1;
          } else {
              last = 0;
          }
          pointer_first--;
      }
      while (pointer_second >= 0) {
          res.num.push_back((other.num[pointer_second] + last) % 10);
          if (other.num[pointer_second] + last >= 10) {
              last = 1;
          } else {
              last = 0;
          }
          pointer_second--;
      }
      if (last) {
          res.num.push_back(last);
      }
      reverse(res.num.begin(), res.num.end());
      return res;
  }

  void operator-=(const LONGNUM& other) {
      *this = *this - other;
  }

  LONGNUM operator-(const LONGNUM& other) const {
      int pointer_first = (int)num.size();
      int pointer_second = (int)other.num.size();
      LONGNUM res;
      pointer_first--, pointer_second--;
      int last = 0;
      while (pointer_first >= 0 && pointer_second >= 0) {
          res.num.push_back((num[pointer_first] - other.num[pointer_second] - last + 10) % 10);
          if (num[pointer_first] < other.num[pointer_second] + last) {
              last = 1;
          } else {
              last = 0;
          }
          pointer_first--, pointer_second--;
      }
      while (pointer_first >= 0) {
          res.num.push_back((num[pointer_first] - last + 10) % 10);
          if (num[pointer_first] < last) {
              last = 1;
          } else {
              last = 0;
          }
          pointer_first--;
      }
      while (pointer_second >= 0) {
          res.num.push_back((other.num[pointer_second] - last + 10) % 10);
          if (other.num[pointer_second] < last) {
              last = 1;
          } else {
              last = 0;
          }
          pointer_second--;
      }
      if (last) {
          res.num.push_back(9);
      }
      while (res.num.size() > 1 && res.num.back() == 0) {
          res.num.pop_back();
      }
      reverse(res.num.begin(), res.num.end());
      return res;
  }

  void operator^=(int pow) {
      *this = *this ^ pow;
  }

  LONGNUM operator^(int pow) const {
      LONGNUM res;
      res.num = {1};
      LONGNUM p;
      p.num = num;
      for (int i = 0; i < pow; ++i) {
          res *= p;
      }
      return res;
  }

  void operator*=(const LONGNUM& other) {
      *this = *this * other;
  }

  LONGNUM operator*(const LONGNUM& other) const {
      LONGNUM res;
      res.num.resize(sz, -1);
      for (int i = 0; i < sz; i++) {
          for (int j = 0; j < sz - i; j++) {
              if (i >= num.size() || j >= other.num.size()) {
                  continue;
              }
              if (res.num[i + j] == -1) {
                  res.num[i + j] = 0;
              }
              res.num[i + j] += num[i] * other.num[j];
          }
      }
      int i = sz - 1;
      while (i >= 0 && res.num[i] == -1) {
          i--;
      }
      for ( ; i > 0 ; i--) {
          res.num[i - 1] += res.num[i] / 10;
          res.num[i] %= 10;
      }
      string e;
      i = 0;
      for ( ; i < sz && res.num[i] != -1; ++i) {
          e += to_string(res.num[i]);
      }
      LONGNUM ans;
      for (auto &t : e) {
          ans.num.push_back(t - '0');
      }
      return ans;
  }

  void operator/=(const LONGNUM& other) {
      *this = *this / other;
  }

  LONGNUM operator/(const LONGNUM& other) const {
      LONGNUM tt;
      tt.num = num;
      LONGNUM res;
      LONGNUM e;
      e.num.push_back(1);
      LONGNUM l;
      l.num.push_back(0);
      LONGNUM r;
      r = tt + e;
      while (r - l > e) {
          LONGNUM md = l + r;
          md.del(2);
          if (md * other > tt) {
              r = md;
          } else {
              l = md;
          }
      }
      return l;
  }

  void operator%=(const LONGNUM& other) {
      *this = *this % other;
  }

  LONGNUM operator%(const LONGNUM& other) const {
      LONGNUM res;
      LONGNUM p;
      p.num = num;
      LONGNUM k = p / other;
      return (p - k * other);
  }
};

class Fraction {
 public:
  LONGNUM numerator;
  LONGNUM denominator;

  void operator+=(const Fraction& other) {
      *this = *this + other;
  }

  Fraction operator+(const Fraction& other) const {
      Fraction res;
      LONGNUM e1 = numerator * other.denominator;
      LONGNUM e2 = other.numerator * denominator;
      res.numerator = numerator * other.denominator + other.numerator * denominator;
      res.denominator = denominator * other.denominator;
      return res;
  }
};

LONGNUM To_LongNum(int e, int mod) {
    LONGNUM res;
    while (e) {
        res.num.push_back(e % mod);
        e /= mod;
    }
    reverse(res.num.begin(), res.num.end());
    return res;
}

int To_Int(LONGNUM k) {
    int ans = 0;
    int pw = 1;
    for (int i = (int)k.num.size() - 1; i >= 0; --i) {
        ans += k.num[i] * pw;
        pw *= 10;
    }
    return ans;
}

Fraction To_Fraction(const LONGNUM& num, const LONGNUM& den) {
    Fraction res;
    res.numerator = num;
    res.denominator = den;
    return res;
}

struct NUMBER {
  LONGNUM integer = {};
  LONGNUM predperiod = {};
  LONGNUM period = {};
};

struct to_ans {
  LONGNUM integer = {};
  Fraction predperiod = {};
  Fraction period = {};
};

struct Dec {
  LONGNUM integer = {};
  Fraction fract = {};
};

struct point {
  int x, y;
};

class To_Decimal {
 public:
  explicit To_Decimal(NUMBER Input_Number, int p) {
      Input_Number_ = std::move(Input_Number);
      p_ = p;
  }

  static LONGNUM INTEGER(LONGNUM& Number, int p) {
      LONGNUM res;
      res.num.push_back(0);
      LONGNUM pt;
      pt.num.push_back(1);
      int pointer = (int)Number.num.size() - 1;
      LONGNUM e;
      LONGNUM BIG_P = To_LongNum(p, 10);
      while (pointer >= 0) {
          e = To_LongNum(Number.num[pointer], 10);
          LONGNUM pp = pt * e;
          res += pt * e;
          pt *= BIG_P;
          pointer--;
      }
      return res;
  }

  static Fraction PREDPERIOD(LONGNUM& Number, int p) {
      Fraction res;
      res.numerator.num = {0};
      res.denominator.num = {1};
      if (Number.num.empty()) {
          return res;
      }
      LONGNUM pow;
      pow = To_LongNum(p, 10);
      for (int t : Number.num) {
          res += To_Fraction(To_LongNum(t, 10), pow);
          pow *= To_LongNum(p, 10);
      }
      return res;
  }

  static Fraction PERIOD(LONGNUM& Number, int sz, int p) {
      Fraction res;
      res.numerator.num = {0};
      res.denominator.num = {1};
      if (Number.num.empty()) {
          return res;
      }
      LONGNUM pow1 = (To_LongNum(p, 10)) ^ sz;
      LONGNUM pow2 = (To_LongNum(p, 10)) ^ (int)Number.num.size();
      LONGNUM num = INTEGER(Number, p);
      LONGNUM e;
      e.num = {1};
      LONGNUM den = pow1 * (pow2 - e);
      res.numerator = num;
      res.denominator = den;
      return res;
  }

  to_ans TOD() {
      to_ans res;
      LONGNUM ans;
      res.integer = INTEGER(Input_Number_.integer, p_);
      res.predperiod = PREDPERIOD(Input_Number_.predperiod, p_);
      res.period = PERIOD(Input_Number_.period, (int)Input_Number_.predperiod.num.size(), p_);
      return res;
  }
 private:
  NUMBER Input_Number_;
  int p_;
};

class TO_Q {
 public:
  explicit TO_Q(Dec& Input_Number, int& q) {
      Input_Number_ = Input_Number;
      q_ = q;
  }

  LONGNUM INTEGER() {
      LONGNUM res;
      LONGNUM integer;
      integer = Input_Number_.integer;
      LONGNUM zero;
      LONGNUM Q = To_LongNum(q_, 10);
      zero.num = {0};
      while (integer > zero) {
          int e = To_Int((integer % Q));
          res.num.push_back(e);
          integer /= Q;
      }
      reverse(res.num.begin(), res.num.end());
      return res;
  }

  string FRACT() {
      vector<int> que;
      vector<int> predperiod;
      vector<int> period;
      Fraction fract = Input_Number_.fract;
      LONGNUM Q = To_LongNum(q_, 10);
      LONGNUM zero;
      zero.num = {0};
      map<vector<int>, bool> used;
      LONGNUM period_num;
      bool flag_period = false;
      int cnt = 0;
      while (fract.numerator > zero) {
          if (used[fract.numerator.num]) {
              period_num = fract.numerator;
              flag_period = true;
              break;
          }
          used[fract.numerator.num] = true;
          fract.numerator *= Q;
          que.push_back(To_Int(fract.numerator / fract.denominator));
          fract.numerator %= fract.denominator;
          cnt++;
      }
      if (!flag_period) {
          for (auto t : que) {
              predperiod.push_back(t);
          }
      } else {
          int i = 0;
          LONGNUM e = Input_Number_.fract.numerator;
          while (i < que.size() && e != period_num) {
              e *= Q;
              e %= fract.denominator;
              predperiod.push_back(que[i]);
              i++;
          }
          while (i < que.size()) {
              period.push_back(que[i]);
              i++;
          }
      }
      string res;
      for (auto t : predperiod) {
          if (t > 9) {
              if (t < 36) {
                  res += char('A' + t - 10);
              } else {
                  res += '[';
                  res += to_string(t);
                  res += ']';
              }
          } else {
              res += to_string(t);
          }
      }
      if (!period.empty()) {
          res += '(';
          for (auto &t : period) {
              if (t > 9) {
                  if (t < 36) {
                      res += char('A' + t - 10);
                  } else {
                      res += '[';
                      res += to_string(t);
                      res += ']';
                  }
              } else {
                  res += to_string(t);
              }
          }
          res += ')';
      }
      return res;
  }

  string TOQ() {
      LONGNUM integer;
      string fract;
      integer = INTEGER();
      fract = FRACT();
      string res;
      for (auto &t : integer.num) {
          if (t > 9) {
              if (t < 36) {
                  res += char('A' + t - 10);
              } else {
                  res += '[';
                  res += to_string(t);
                  res += ']';
              }
          } else {
              res += to_string(t);
          }
      }
      if (integer.num.empty()) {
          res += '0';
      }
      if (!fract.empty()) {
          res += '.';
          res += fract;
      }
      return res;
  }

 private:
  Dec Input_Number_;
  int q_;
};


class BackEnd {
 public:
  explicit BackEnd(string Input_Number) : Input_Number_(std::move(Input_Number)) {}
  void Push_Back(LONGNUM& arr, int& start) {
      if (Input_Number_[start] >= 'A' && Input_Number_[start] <= 'Z') {
          int p = Input_Number_[start] - 'A';
          arr.num.push_back(p + 10);
      } else if (Input_Number_[start] == '[') {
          start++;
          vector<int> TT;
          while (start < Input_Number_.size() && Input_Number_[start] != ']') {
              TT.push_back(Input_Number_[start]);
              start++;
          }
          int num = 0, pr = 1;
          for (int i = (int)TT.size() - 1; i >= 0; --i) {
              num += pr * (TT[i] - '0');
              pr *= 10;
          }
          arr.num.push_back(num);
      } else {
          arr.num.push_back(int(Input_Number_[start] - '0'));
      }
  }
  NUMBER Rec() {
      LONGNUM integer;
      int start = 0;
      for ( ; start < Input_Number_.size() && Input_Number_[start] != '.' && Input_Number_[start] != ','; ++start) {
          Push_Back(integer, start);
      }
      start++;
      LONGNUM predperiod;
      LONGNUM period;
      for ( ; start < Input_Number_.size() && Input_Number_[start] != '('; ++start) {
          Push_Back(predperiod, start);
      }
      start++;
      for ( ; start < Input_Number_.size() && Input_Number_[start] != ')'; ++start) {
          Push_Back(period, start);
      }
      return {integer, predperiod, period};
  }
  static Dec To_D(NUMBER& Out_Number, int p) {
      To_Decimal ans(Out_Number, p);
      to_ans pt = ans.TOD();
      Dec Number;
      Number.integer = pt.integer;
      LONGNUM one;
      one.num = {1};
      Number.fract = pt.predperiod + pt.period;
      Number.integer += (Number.fract.numerator / Number.fract.denominator);
      Number.fract.numerator %= Number.fract.denominator;
      return Number;
  }
 private:
  string Input_Number_;
};

string Main_Project(string& input, int p, int q) {
    BackEnd BACKEND(input);
    NUMBER ER = BACKEND.Rec();
    Dec Dec_Num = BackEnd::To_D(ER, p);
    TO_Q tt(Dec_Num, q);
    string res = tt.TOQ();
    return res;
}

class FrontEnd {
 public:
  static void DrawRectangle(RenderWindow& Main_Project_Window, int coord_x, int coord_y, int size_x, int size_y) {
      RectangleShape Rectangle;
      Rectangle.setPosition(float(coord_x), float(coord_y));
      Rectangle.setSize(Vector2f(float(size_x), float(size_y)));
      Main_Project_Window.draw(Rectangle);
  }

  void Which_Window() {
      if (mouse_pos_.x >= left_coord_ && mouse_pos_.x <= 765 + 150) {
          if (mouse_pos_.y >= 150 && mouse_pos_.y <= 200) {
              Window_ = 0;
          } else if (mouse_pos_.y >= 350 && mouse_pos_.y <= 400) {
              Window_ = 1;
          } else if (mouse_pos_.y >= 550 && mouse_pos_.y <= 600) {
              Window_ = 2;
          } else if (mouse_pos_.y >= 750 && mouse_pos_.y <= 800) {
              Window_ = 3;
          }
      } else {
          Window_ = -1;
      }
      click_ = false;
  }

  static void update_num(Event Main_Project_Window_Event, string& r) {
      if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
          if (!r.empty()) {
              r.pop_back();
          }
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
          r += '0';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
          r += '1';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
          r += '2';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
          r += '3';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
          r += '4';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
          r += '5';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
          r += '6';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
          r += '7';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
          r += '8';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
          r += '9';
      }
  }

  static void update_str(Event Main_Project_Window_Event, string& r) {
      if (Main_Project_Window_Event.type == Event::KeyPressed &&
          Keyboard::isKeyPressed(sf::Keyboard::BackSpace)) {
          if (!r.empty()) {
              r.pop_back();
          }
      }
      bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
      if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::A)) {
          r += 'A';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::B)) {
          r += 'B';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::C)) {
          r += 'C';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::D)) {
          r += 'D';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::E)) {
          r += 'E';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::F)) {
          r += 'F';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::G)) {
          r += 'G';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::H)) {
          r += 'H';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::I)) {
          r += 'I';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::J)) {
          r += 'J';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::K)) {
          r += 'K';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::L)) {
          r += 'L';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::M)) {
          r += 'M';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::N)) {
          r += 'N';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::O)) {
          r += 'O';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::P)) {
          r += 'P';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Q)) {
          r += 'Q';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::R)) {
          r += 'R';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::S)) {
          r += 'S';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::T)) {
          r += 'T';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::U)) {
          r += 'U';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::V)) {
          r += 'V';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::W)) {
          r += 'W';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::X)) {
          r += 'X';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Y)) {
          r += 'Y';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Z)) {
          r += 'Z';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::LBracket)) {
          r += '[';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::RBracket)) {
          r += ']';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num0)) {
          if (!shift) {
              r += '0';
          } else {
              r += ')';
          }
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
          r += '1';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
          r += '2';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
          r += '3';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
          r += '4';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
          r += '5';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
          r += '6';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
          r += '7';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
          r += '8';
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
          if (!shift) {
              r += '9';
          } else {
              r += '(';
          }
      } else if (Main_Project_Window_Event.type == Event::KeyPressed
          && Keyboard::isKeyPressed(sf::Keyboard::Period)) {
          r += '.';
      }
  }

  void print_Window_0(RenderWindow& Main_Project_Window) {
      Text output;
      output.setFont(Main_Font_);
      string e;
      float cnt = 0;
      for (auto &t : input_) {
          if (t == '[' || t == ']' || t == '(' || t == ')'
              || t == '.') {
              cnt += 0.1;
          } else {
              cnt += 1;
          }
      }
      if ((int)cnt <= str_sz_) {
          e = input_;
      } else {
          for (int i = start_Window_0_; i <= min((int)input_.size() - 1, start_Window_0_ + str_sz_); ++i) {
              e += input_[i];
          }
      }
      output.setString(e);
      output.setCharacterSize(50);
      output.setPosition(float(left_coord_), 150 - 10);
      output.setFillColor(Color::Red);
      Main_Project_Window.draw(output);
  }

  void print_Window_1(RenderWindow& Main_Project_Window) {
      Text output;
      output.setFont(Main_Font_);
      string e;
      if (p_.size() <= str_sz_) {
          e = p_;
      } else {
          for (int i = start_Window_1_; i <= min((int)p_.size() - 1, start_Window_1_ + str_sz_); ++i) {
              e += p_[i];
          }
      }
      output.setString(e);
      output.setCharacterSize(50);
      output.setPosition(float(left_coord_), 350 - 10);
      output.setFillColor(Color::Red);
      Main_Project_Window.draw(output);
  }

  void print_Window_2(RenderWindow& Main_Project_Window) {
      Text output;
      output.setFont(Main_Font_);
      string e;
      if (q_.size() <= str_sz_) {
          e = q_;
      } else {
          for (int i = start_Window_2_; i <= min((int)q_.size() - 1, start_Window_2_ + str_sz_); ++i) {
              e += q_[i];
          }
      }
      output.setString(e);
      output.setCharacterSize(50);
      output.setPosition(float(left_coord_), 550 - 10);
      output.setFillColor(Color::Red);
      Main_Project_Window.draw(output);
  }

  void print_Window_3(RenderWindow& Main_Project_Window) {
      Text output;
      output.setFont(Main_Font_);
      string e;
      float cnt = 0;
      for (auto &t : output_) {
          if (t == '[' || t == ']' || t == '(' || t == ')'
              || t == '.') {
              cnt += 0.1;
          } else {
              cnt += 1;
          }
      }
      if ((int)cnt <= str_sz_) {
          e = output_;
      } else {
          for (int i = start_Window_3_; i <= min((int)output_.size() - 1, start_Window_3_ + str_sz_); ++i) {
              e += output_[i];
          }
      }
      output.setString(e);
      output.setCharacterSize(50);
      output.setPosition(float(left_coord_), 750 - 10);
      output.setFillColor(Color::Red);
      Main_Project_Window.draw(output);
  }

  void print_Window_4(RenderWindow& Main_Project_Window) {
      Text input;
      input.setFont(Main_Font_);
      string e = "Upload";
      input.setString(e);
      input.setCharacterSize(50);
      input.setFillColor(Color::Blue);
      input.setPosition(float(left_coord_) + 125, 911);
      Main_Project_Window.draw(input);
  }

  void print_Window_5(RenderWindow& Main_Project_Window) {
      Text output;
      output.setFont(Main_Font_);
      output.setCharacterSize(50);
      string e = "Download";
      output.setString(e);
      output.setFillColor(Color::Blue);
      output.setPosition(float(left_coord_) + 455, 911);
      Main_Project_Window.draw(output);
  }

  void Making_Text(Text& text, string e, pair<int, int> pos) {
      text.setFont(Main_Font_);
      text.setFillColor(Color::Green);
      text.setPosition(float(pos.first), float(pos.second));
      text.setCharacterSize(50);
      text.setString(e);
  }

  void print_text(RenderWindow& Main_Project_Window) {
      Text input;
      Making_Text(input, "Input number:", {150, 90});
      Text p;
      Making_Text(p, "p:", {150, 290});
      Text q;
      Making_Text(q, "q:", {150, 490});
      Text output;
      Making_Text(output, "Output number:", {150, 690});
      Main_Project_Window.draw(input);
      Main_Project_Window.draw(p);
      Main_Project_Window.draw(q);
      Main_Project_Window.draw(output);
  }

  void check() {
      max_input_number_ = 0;
      for (int i = 0; i < (int)input_.size(); ++i) {
          if (input_[i] == '[') {
              int num = 0, cnt = 1;
              i++;
              while (i < (int) input_.size() && input_[i] != ']') {
                  num += cnt * ((input_[i] - '0') % 10);
                  cnt *= 10;
                  i++;
              }
              max_input_number_ = max(max_input_number_, num);
              i++;
          } else {
              max_input_number_ = max(max_input_number_, input_[i] - '0');
          }
      }
      if (input_.empty()) {
          output_ = "Error: Input number is empty";
      } else if (p_.empty()) {
          output_ = "Error: P is empty";
      } else if (q_.empty()) {
          output_ = "Error: Q is empty";
      } else if (max_input_number_ >= stoi(p_)) {
          output_ = "Error: Input number >= P";
      } else {
          output_ = Main_Project(input_, stoi(p_), stoi(q_));
      }
  }

  void upload() {
      string new_p;
      string new_q;
      string new_input_number;
      ifstream input(upload_file_);
      input >> new_input_number;
      input >> new_p;
      input >> new_q;
      input_ = new_input_number;
      p_ = new_p;
      q_ = new_q;
      check();
      input.close();
  }

  void download() {
      ofstream output(download_file_);
      output << "Input number: " << input_ << '\n';
      output << "P: " << p_ << '\n';
      output << "Q: " << q_ << '\n';
      output << "Output number: " << output_;
      output.close();
  }

  void Print_Windows(RenderWindow& Main_Project_Window) {
      print_Window_0(Main_Project_Window);
      print_Window_1(Main_Project_Window);
      print_Window_2(Main_Project_Window);
      print_Window_3(Main_Project_Window);
      print_Window_4(Main_Project_Window);
      print_Window_5(Main_Project_Window);
  }

  void Draw_Rectangles(RenderWindow& Main_Project_Window) {
      DrawRectangle(Main_Project_Window, left_coord_, 150, 765, 50);
      DrawRectangle(Main_Project_Window, left_coord_, 350, 765, 50);
      DrawRectangle(Main_Project_Window, left_coord_, 550, 765, 50);
      DrawRectangle(Main_Project_Window, left_coord_, 750, 765, 50);
      DrawRectangle(Main_Project_Window, left_coord_ + 20, 920, 355, 50);
      DrawRectangle(Main_Project_Window, left_coord_ + 385, 920, 355, 50);
  }

  void Main_FrontEnd() {
      RenderWindow Main_Project_Window(VideoMode(MAIN_PROJECT_WINDOW_W_, MAIN_PROJECT_WINDOW_H_), "Zachemtakbolno");
      Main_Font_.loadFromFile("Romance.ttf");
      while (Main_Project_Window.isOpen()) {
          Event Main_Project_Window_Event{};
          while (Main_Project_Window.pollEvent(Main_Project_Window_Event)) {
              if (Main_Project_Window_Event.type == Event::Closed) {
                  Main_Project_Window.close();
              } else if (Main_Project_Window_Event.type == Event::MouseButtonPressed &&
                  Main_Project_Window_Event.mouseButton.button == Mouse::Left) {
                  mouse_pos_ = Mouse::getPosition(Main_Project_Window);
                  //cout << mouse_pos_.x << ' ' << mouse_pos_.y << '\n';
                  click_ = true;
              } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
                  Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                  check();
              } else {
                  if (Window_ == -1) {
                      break;
                  } else if (Window_ == 0) {
                      if (Main_Project_Window_Event.type == Event::KeyPressed &&
                      Keyboard::isKeyPressed(Keyboard::Right)) {
                          if (input_.size() >= str_sz_) {
                              start_Window_0_++;
                          } else {
                              start_Window_0_ = 0;
                          }
                          //cout << start_Window_0_ << '\n';
                      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(Keyboard::Left)) {
                          if (input_.size() >= str_sz_) {
                              start_Window_0_ = max(0, start_Window_0_ - 1);
                          } else {
                              start_Window_0_ = 0;
                          }
                          //cout << start_Window_0_ << '\n';
                      } else {
                          update_str(Main_Project_Window_Event, input_);
                      }
                      //cout << input_ << '\n';
                  } else if (Window_ == 1) {
                      if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(Keyboard::Right)) {
                          if (p_.size() >= str_sz_) {
                              start_Window_1_++;
                          } else {
                              start_Window_1_ = 0;
                          }
                          //cout << start_Window_1_ << '\n';
                      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(Keyboard::Left)) {
                          if (p_.size() >= str_sz_) {
                              start_Window_1_ = max(0, start_Window_1_ - 1);
                          } else {
                              start_Window_1_ = 0;
                          }
                          //cout << start_Window_1_ << '\n';
                      } else {
                          update_num(Main_Project_Window_Event, p_);
                      }
                  } else if (Window_ == 2) {
                      if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(Keyboard::Right))   {
                          if (q_.size() >= str_sz_) {
                              start_Window_2_++;
                          } else {
                              start_Window_2_ = 0;
                          }
                          //cout << start_Window_2_ << '\n';
                      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(Keyboard::Left)) {
                          if (q_.size() >= str_sz_) {
                              start_Window_2_ = max(0, start_Window_2_ - 1);
                          } else {
                              start_Window_2_ = 0;
                          }
                          //cout << start_Window_2_ << '\n';
                      } else {
                          update_num(Main_Project_Window_Event, q_);
                      }
                  } else if (Window_ == 3) {
                      if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(Keyboard::Right)) {
                          if (output_.size() >= str_sz_) {
                              start_Window_3_++;
                          } else {
                              start_Window_3_ = 0;
                          }
                          //cout << start_Window_2_ << '\n';
                      } else if (Main_Project_Window_Event.type == Event::KeyPressed &&
                          Keyboard::isKeyPressed(Keyboard::Left)) {
                          if (output_.size() >= str_sz_) {
                              start_Window_3_ = max(0, start_Window_3_ - 1);
                          } else {
                              start_Window_3_ = 0;
                          }
                          //cout << start_Window_2_ << '\n';
                      }
                  }
              }
          }
          if (click_) {
              Which_Window();
              if (mouse_pos_.y >= 920 && mouse_pos_.y <= 970) {
                  if (mouse_pos_.x >= left_coord_ + 20 && mouse_pos_.x <= left_coord_ + 375) {
                      upload();
                  } else if (mouse_pos_.x >= left_coord_ + 385 && mouse_pos_.x <= left_coord_ + 385 + 355) {
                      download();
                  }
              }
              //cout << Window_ << '\n';
          }
          Main_Project_Window.clear();
          Draw_Rectangles(Main_Project_Window);
          Print_Windows(Main_Project_Window);
          print_text(Main_Project_Window);
          Main_Project_Window.display();
      }
  }
 private:
  Font Main_Font_;
  string input_;
  string p_;
  string q_;
  int max_input_number_ = 0;
  string output_ = "Empty";
  const int MAIN_PROJECT_WINDOW_H_ = 1010;
  const int MAIN_PROJECT_WINDOW_W_ = 1010;
  const int str_sz_ = 26;
  int left_coord_ = 150;
  int Window_ = -1;
  Vector2i mouse_pos_;
  bool click_= false;
  int start_Window_0_ = 0;
  int start_Window_1_ = 0;
  int start_Window_2_ = 0;
  int start_Window_3_ = 0;
  string upload_file_ = "/home/pentaghoul/ProjectF/Project_Input";
  string download_file_ = "/home/pentaghoul/ProjectF/Project_Output";
};

string MN(string& input, int p, int q) {
    BackEnd BACKEND(input);
    NUMBER ER = BACKEND.Rec();
    Dec Dec_Num = BackEnd::To_D(ER, p);
    TO_Q tt(Dec_Num, q);
    string res = tt.TOQ();
    return res;
}

int main() {
    string input;
    cin >> input;
    int p, q;
    cin >> p >> q;
    string res = MN(input, p, q);
    cout << res;
    return 0;
    FrontEnd Window;
    Window.Main_FrontEnd();
    return 0;
}
