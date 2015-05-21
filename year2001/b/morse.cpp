// morse.cc
//
// Adam Florence
//
// Solves the Mutilated Morse Code problem.

#include<iostream>
#include<string>

using namespace std;

// Morse code mapping
const string text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_,.?";
const string morse[] = { ".-",     // A
                         "-...",   // B
                         "-.-.",   // C
                         "-..",    // D
                         ".",      // E
                         "..-.",   // F
                         "--.",    // G
                         "....",   // H
                         "..",     // I
                         ".---",   // J
                         "-.-",    // K
                         ".-..",   // L
                         "--",     // M
                         "-.",     // N
                         "---",    // O
                         ".--.",   // P
                         "--.-",   // Q
                         ".-.",    // R
                         "...",    // S
                         "-",      // T
                         "..-",    // U
                         "...-",   // V
                         ".--",    // W
                         "-..-",   // X
                         "-.--",   // Y
                         "--..",   // Z
                         "..--",   // underscore
                         ".-.-",   // comma
                         "---.",   // period
                         "----"};  // question mark

// Convert text to Morse code and number string.
void text_to_morse(const string& t, string& m, string& n);

// Convert Morse code and number string to text.
void morse_to_text(const string& m, const string& n, string& t);

// reverse a string
string reverse(const string& s);

// ----------------------------------------------------------------------- //
// MAIN

int main(void)
{
  // Read number of lines to process
  int num;  // number of lines to process
  cin >> num;

  for (int i = 1; i <= num; ++i)
    {
      // Read the encrypted message
      string e;  // encrypted message
      cin >> e;

      // Convert to Morse code and number string
      string m;  // Morse code message without pauses
      string n;  // number string
      text_to_morse(e, m, n);

      // Reverse number string
      string r = reverse(n);

      // Convert to text
      string t;  // decrypted message
      morse_to_text(m, r, t);

      // Print decrypted message
      cout << i << ": " << t << endl;
    }

  return 0;
}

// ----------------------------------------------------------------------- //
// FUNCTIONS

// Convert character to Morse code.
string char_to_morse(const char c)
{
  if (('A' <= c) && (c <= 'Z'))
    return morse[c - 'A'];
  if (c == '_')
    return morse[26];
  if (c == ',')
    return morse[27];
  if (c == '.')
    return morse[28];
  if (c == '?')
    return morse[29];

  // Else, character is not valid
  cout << "Error! The character '" << c << "' is not valid." << endl;
  exit(1);
}

// Convert text to Morse code and number string.
void text_to_morse(const string& t, string& m, string& n)
{
  m = "";
  n = "";
  for (int i = 0; i < t.length(); ++i)
    {
      string s = char_to_morse(t[i]);
      m += s;
      n += ('0' + s.length());
    }
}

// Convert Morse code to character.
char morse_to_char(const string& m)
{
  for (int i = 0; i < 32; ++i)
    if (morse[i] == m)
      return text[i];

  // Else, Morse code string is not valid
  cout << "Error! Morse code string '" << m << "' is not valid." << endl;
  exit(1);
}

// Convert Morse code and number string to text.
void morse_to_text(const string& m, const string& n, string& t)
{
  t = "";
  int z = 0;
  for (int i = 0; i < n.length(); ++i)
    {
      string s = "";
      const int num = n[i] - '0';
      for (int j = 0; j < num; ++j)
        s += m[z++];
      t += morse_to_char(s);
    }
}

// Reverse a string
string reverse(const string& s)
{
  string r = "";
  for (int i = s.length() - 1; i >= 0; --i)
    r += s[i];
  return r;
}

