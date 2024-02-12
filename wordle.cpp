// Priyanshu Luhar
// 22 NOV 2022

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class Words
{
    private:
        int count, wordlen;
        string * matches;

        void create_array()
        {
            if(* matches != "NULL")
            {
                delete [] matches;
            }

            int size = count_words();
            matches = new string[size];
        }
    public:
        Words(int length)
        {
            count = 0;
            wordlen = length;
            matches = new string;
            * matches = "NULL";

            get_words();
        }

        ~Words()
        {
            delete [] matches;
        }

        int count_words()
        {

            ifstream wordfile;

            wordfile.open("enable1.txt");

            if(wordfile.is_open())
            {
                count = 0;
                while(!wordfile.eof())
                {
                    string word; 
                    wordfile >> word;

                    if(wordlen == word.length())
                    {
                        count++;
                    }
                }
            }
            wordfile.close();
            return count;
        }

        void get_words()
        {
            create_array();

            ifstream wordfile;
            wordfile.open("enable1.txt");

            if(wordfile.is_open())
            {
                int i=0;
                while(!wordfile.eof())
                {
                    string word;
                    wordfile >> word;

                    if(wordlen == word.length())
                    {
                        matches[i] = word;
                        i++;
                    }
                }
            }
            wordfile.close();
        }

        int get_count()
        {
            return count;
        }

        bool is_valid(string word)
        {
            bool not_found = true;
            for(int i=0; i<count && not_found; i++)
            {
                if(matches[i] == word)
                {
                    not_found = false;
                }
            }
            return !not_found;
        }

        string pick_word()
        {
            int x = rand() % count;
            return matches[x];
        }

        void reload(int wl)
        {
            wordlen = wl;
            count = 0;
            get_words();
        }
};

class Wordle
{
    protected:
        char target[40];
        char lastguess[40];
        string evals[10];
        string guesses[10];
        int attempts;
        int max_attempts;
        bool onlyvalid;
        char usable[40];

    private:
        bool is_char_in_word(string word, char c)
        {
            bool not_found = true;
            char array[word.length() + 1];
            strcpy(array, word.c_str());

            for(int i=0; i<word.length() && not_found; i++)
            {
                if(c == array[i])
                {
                    not_found = false;
                }
            }
            return !not_found;
        }

        void evaluate_guess(char result[], const char guess[])
        {
            strcpy(result, guess);

            for(int i=0; i<strlen(result); i++)
            {
                result[i] = '-';
                toupper(guess[i]);

            }

            int i=0;
            bool found = false;

            for(int i = 0; i < strlen(guess); i++)
            {
                if(guess[i] == target[i])
                {
                    result[i] = ' ';

                    for(int k=0; k<26; k++)
                    {
                        if(guess[i] == usable[k])
                        {
                            usable[k] = usable[k] - 32;
                        }
                    } 
                }
                else 
                {
                    for(int j = 0; j < strlen(target); j++)
                    {
                        if(guess[i] == target[j])
                        {
                            result[i] = '*';
                            for(int k=0; k<26; k++)
                            {
                                if(guess[i] == usable[k])
                                {
                                    usable[k] = usable[k] - 32;
                                }
                            } 
                        }
                    }
                }

                if(result[i] == '-')
                {
                    result[i] = '!';
                    for(int k=0; k<26; k++)
                    {
                        if(guess[i] == usable[k])
                        {
                            usable[k] = ' ';
                        }
                    } 
                }
            }
        }

    public:
        Wordle(string secret, int ma, bool ov)
        {
            attempts = 0;
            strcpy(target, secret.c_str());
            onlyvalid = ov;
            max_attempts = ma;
            for(int i=0; i<26; i++)
            {
                usable[i] = char(97+i);
            } 
        }

        bool guessed_it()
        {
            return !strcmp(lastguess, target);
        } 

        bool game_over()
        {
            bool done = false;

            if(attempts >= max_attempts)
            {
                done = true;
            }

            if(guessed_it())
            {
                done = true;
            }

            return done;
        }

        void set_onlyvalid(bool ov)
        {
            onlyvalid = ov;
        }

        bool get_onlyvalid()
        {
            return onlyvalid;
        }

        void set_max_attempts(int ma)
        {
            max_attempts = ma;
        }

        bool submit_guess(string guess)
        {
            bool not_going = false;

            if(attempts < max_attempts)
            {
                not_going = true;
            }

            char eval[40];

            strcpy(lastguess, guess.c_str());

            guesses[attempts] = guess;

            evaluate_guess(eval, guess.c_str());

            evals[attempts] = eval;

            attempts++;

            return not_going;
        }
};

class WordleConsole : public Wordle
{
    private:
        bool showlegend;


        string displayed_guess(int guessindex)
        {
            string result = "";
            string guess = guesses[guessindex];
            string eval = evals[guessindex];
            for (int i = 0; i < strlen(target); i++)
            {
                string par = "\033[1;33m";
                string cor = "\033[1;34m";
                string wrn = "\033[1;31m";
                string pre;
                if(eval[i] == ' ')
                {
                    guess[i] = guess[i] - 32;
                    pre = cor;
                }
                else if(eval[i] == '*')
                {
                    pre = par;
                }
                else
                {
                    pre = wrn;
                }
                
                result = result + pre +  guess[i] + "\033[0m" + " ";
            }
            return result;
        }

    public:

        WordleConsole(string secret, int max_attempts, bool onlyvalid, bool legend = true) : Wordle(secret, max_attempts, onlyvalid)
    {
        showlegend = legend;
    }
        void set_showlegend(bool sl)
        {
            showlegend = sl;
        }

        friend ostream & operator << (ostream & out, WordleConsole& obj)
        {
            string le = "\033[1;36m---------- W O R D L E ----------\033[0m\n";
            
            if(obj.showlegend)
            {
                le = le + "\n\033[1;37mGAME PLAY LEGEND \033[0m\n  \033[1;33mYellow\033[0m - Letter is in the wrong spot \n  \033[1;31mRed\033[0m  - Letter is not in the word\033[0m\n  \033[1;34mBlue\033[0m - Letter is in the correct spot \n";
            }
            out << le << endl;

            for(int i=0; i<obj.attempts; i++)
            {
                out << "\t" << to_string(i+1) << ") " << obj.displayed_guess(i) << endl;
            }
            out << "Usable letters:\n\t ";
            for(int i=0; i<26; i++)
            {
                out << "\033[35m" << obj.usable[i] << "\033[0m" << ", " ;
            }
            out << endl;

            return out;
        }
};

int get_wordlength(int min, int max)
{
    int length;

    cout << "Enter the length of word: ";
    cin >> length;

    if(length > max || length < min)
    {
        bool correct = false;
        while(!correct)
        {
            cout << "\nERROR: Between " << min-1 << " and " << max+1 << "\n" << endl;
            cout << "Enter the length of word: ";
            cin >> length;
            if(length < max && length > min)
            {
                correct = true;
            }
        }
    }

    return length;
}

int main()
{
    srand(time(NULL));


    system("clear");
    cout << "\033[1;36m---------- W O R D L E ----------\033[0m\n\n" << endl;
    cout << "\tc: Classic mode (6 attempts)" << endl; 
    cout << "\te: Easy mode (10 attempts)" << endl; 
    cout << "\th: Hard mode (4 attempts)" << endl; 
    cout << "\tl: Legend toggle (6 attempts) " << endl; 
    cout << "\t2: Two-Player mode (6 attempts)" << endl; 
    cout << "\tq: Quit" << endl; 
    cout << endl;

    char input;
    cout << "Choose the mode: ";
    cin >> input;

    int max_attempts;
    bool onlyvalid = true;
    bool showlegend = true;
    string targetword;

    switch(input)
    {
        case 'c' : max_attempts=6;
                   break;

        case 'e' : max_attempts=10;
                   break;

        case 'h' : max_attempts=4;
                   break;

        case 'l' : max_attempts=6;
                   showlegend = false;
                   break;

        case '2' : max_attempts=6;
                   onlyvalid = false;
                   cout << "\nEnter secret word: ";
                   cin >> targetword;
                   break;

        case 'q' : cout << "\nPeace out." << endl;
                   return 0;
                   break;
    }


    int wordlength;
    bool allowsubmit;

    cout << endl;
    if(input != '2')
        wordlength = get_wordlength(4, 8);

    if(input != '2')
    {
        

        cout << "\nWORDLE is preparing word list" << endl << endl;
    }
    system("sleep 1s");
    system("clear");

    cout << "\n\n\t -- READY !! --" << endl;
    system("sleep 1s");
    system("clear");

    cout << "\n\n\t -- SET !! --" << endl;
    system("sleep 1s");
    system("clear");

    cout << "\n\n\t -- GO !! --" << endl;
    system("sleep 1s");
    system("clear");

    string guess;
    Words gamewords(wordlength);
    if(input != '2')
        targetword = gamewords.pick_word();

    WordleConsole game(targetword, max_attempts, onlyvalid);
    game.set_showlegend(showlegend);
    cout << game << endl;

    while(!game.game_over())
    {
        cout << "Enter guess: ";
        cin >> guess;

        system("clear");
        if (game.get_onlyvalid())
            allowsubmit = gamewords.is_valid(guess);
        else
            allowsubmit = true;

        if (allowsubmit)
        {
            if (guess.length() != targetword.length())
                cout << "\n" << guess << " doesn't have enough characters" << endl;
            else
                game.submit_guess(guess);
        } 
        else
            cout << "\n\n" << guess << " is not a valid word" << endl;

        cout << game << endl << endl;
    }

    cout << "GAME OVER -- YOU ";
    if(game.guessed_it())
    {
        cout << "WON !!" << endl;
    }
    else
    {
        cout << "LOST..." << endl;
        cout << "The correct word was " << targetword << endl;
    }

    return 0;
}
