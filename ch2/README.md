#String Handling

----
###StringAlgorithms

using boost::algorithm<br/>

1. ::to_upper/::to_lower<br/>
如同字面上意思

2. erase_*_copy<br/>
刪除字串/數字

3. ::find_*<br/>
找字串

4. ::join<br/>
把container內的字串合成到一個新的C++ String.

5. ::replace_*_copy<br/>
取代字串

6. ::trim_*_copy<br/>
刪除空白字元

7. ::is_any_of<br/>
做出filter的條件(用在切割字串)

8. ::is_digit/::is_upper/::is_lower<br/>
如同字面上意思

9. ::starts_with/::ends_with/::contains/::lexicographical_compare<br/>
如同字面上意思

10. ::split<br/>
如同字面上意思

11. ::find_regex<br/>
找regex(regular expression)

----
###LexicalCast

1. ::lexical_cast<br/>
like as std::stoi(), std::stod, std::to_string(), it's throw exception when the input can't be converted.

----
####Format

1. ::format<br/>
A powerful string converted function.<br/>
ex: std::cout << boost::format{"%1%.%2%.%3%"} % 12 % 5 % 2014 << "\n" ;
````
struct animal
{
  std::string name;
  int legs;
};

std::ostream &operator<<(std::ostream &os, const animal &a)
{
  return os << a.name << ',' << a.legs;
}

int main()
{
  animal a{"cat", 4};
  std::cout << boost::format{"%1%"} % a << '\n';
}
````

----
####Regex

Due to I don't understand regular expression, skip this chapter.

----
####Xpressive

Due to I don't understand regular expression, skip this chapter.

----
####Tokenizer

1. ::tokenizer
````
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  std::string s = "Boost C++ Libraries";
  // if not give separator, using punctuation as default. 
  boost::char_separator<char> sep{" "};
  tokenizer tok{s, sep};
  for (const auto &t : tok)
    std::cout << t << '\n';
````

2. ::char_separator
````
std::string s = "Boost C++ Libraries" ;
boost::char_separator<char> sep{" "}; // 以空白當分割，並且空白"不當作"一個token
>> Boost C++ Libraries
boost::char_separator<char> sep{" ", "+"}; // 以+號當分割，並且也把他當成一個token
>> Boost C + + Libraries
boost::char_separator<char> sep{" ", "+", boost::keep_empty_tokens};
//empty token's policy
````

3. ::escaped_list_separator
把\"號當成separator
````
  typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
  std::string s = "Boost,\"C++ Libraries\"";
  tokenizer tok{s};
  >> Boost
  >> C++ Libraries 
````

4. ::offset_separator
利用string offset當作separator
````
  typedef boost::tokenizer<boost::offset_separator> tokenizer;
  std::string s = "Boost C++ Libraries";
  int offsets[] = {5, 5, 9};
  boost::offset_separator sep{offsets, offsets + 3};
  tokenizer tok{s, sep};
  >> Boost
  >>  C++ 
  >> Libraries
````






