/* ~~ Hash Code 2020 Score Calculator by Foxel ~~ */
/* ~~~~~~~~~~~~~ Mono-File Version ~~~~~~~~~~~~~~ */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

struct library
{
	unsigned int booksnum = 0;	//max = 10^5
	std::vector<unsigned int> books;
};

struct out_library : library
{
	inline static unsigned int libnum;

	unsigned int code = 0;	//max = 10^5
	//int booksnum
	//vector books
};

struct in_library : library
{
	inline static unsigned int totalbooks = 0;
	inline static unsigned int libs = 0;
	inline static unsigned int days = 0;
	inline static std::vector<unsigned short> scores;

	//int booksnum
	unsigned int signup = 0;	//max = 10^5
	unsigned int ship = 0;	//max = 10^5
	//vector books
};

template <typename Functor, typename P>
void insert(Functor add, P& pos, std::vector<char>& temp)
{
	unsigned int pow = 1;

	for (size_t tmp = temp.size(); tmp > 0; tmp--)
	{
		add() += temp[tmp - 1] * pow;
		pow *= 10;
	}
	pos++;
	temp.clear();
}

template <typename Functor, typename P>
void read(const std::string& line, Functor add, P& pos)
{
	std::vector<char> temp;

	for (auto it : line)
	{
		if (it != ' ') temp.push_back(it - '0');
		else insert(add, pos, temp);
	}
	insert(add, pos, temp);
}

void printin(const std::vector<out_library>& libraries)
{
	std::cout << std::endl << "Libraries: " << out_library::libnum << std::endl;

	for (const auto& print : libraries)
	{
		std::cout << "Library: " << print.code << " - Books: " << print.booksnum << std::endl << "List of books: ";

		for (auto book : print.books) std::cout << book << " ";
		std::cout << std::endl;
	}
}

void printout(std::vector<in_library>& models)
{
	std::cout << std::endl << "Books: " << in_library::totalbooks << " - Libraries: " << in_library::libs << " - Days: " << in_library::days << std::endl << "Book scores: ";

	for (auto print : in_library::scores) std::cout << print << " ";
	std::cout << std::endl;

	unsigned int n = 0;
	for (const auto& print : models)
	{
		std::cout << "Library: " << n++ << " Books: " << print.booksnum << " - Signup time: " << print.signup << " - Ship time: " << print.ship << std::endl << "List of books: ";

		for (auto book : print.books) std::cout << book << " ";
		std::cout << std::endl;
	}
}

void close(std::ifstream& sub, std::ifstream& in)
{
	sub.close();
	sub.clear();
	in.close();
	in.clear();
}

bool calculate(std::ifstream& sub, const std::string& subpath, std::ifstream& in, const std::string& inpath)
{
	//opening files
	std::clog << "Opening " << subpath << "..." << std::endl;

	sub.open(subpath, std::ios_base::in);
	in.open(inpath, std::ios_base::in);

	try
	{
		if (sub)	//files found
		{
			unsigned int pos = 0;


			//reading submission
			std::clog << "Reading submission..." << std::endl;

			std::vector<out_library> libraries;

			try {
				{
					//first line
					std::string s_libraries;
					getline(sub, s_libraries);

					out_library::libnum = stoi(s_libraries);
				}

				//libraries
				libraries.resize(out_library::libnum);

				for (auto& it : libraries)
				{
					std::string line;

					{
						//first library line
						getline(sub, line);

						pos = 0;
						read(line, [&]() ->unsigned int& { return (!pos ? it.code : it.booksnum); }, pos);
					}
					{
						//library list of books
						getline(sub, line);
						it.books.resize(it.booksnum, 0);

						pos = 0;
						read(line, [&]() ->unsigned int& { return (it.books[pos]); }, pos);
					}
				}
			}
			catch (...)
			{
				throw std::runtime_error("Unknown format error: put only numeric data inside the file");
			}


			//reading data
			std::clog << "Reading data..." << std::endl;

			std::vector<in_library> models;

			try {
				if (!in) throw std::runtime_error("Please do not delete or rename .hashcode data files");

				//first line
				std::string line;
				getline(in, line);

				pos = 0;
				read(line, [&]() ->unsigned int& { return (!pos ? in_library::totalbooks : (!(pos - 1) ? in_library::libs : in_library::days)); }, pos);

				if (out_library::libnum <= in_library::libs) //valid file
				{
					//scores
					pos = 0;
					in_library::scores.resize(in_library::totalbooks, 0);

					getline(in, line);
					read(line, [&]() ->unsigned short& { return (in_library::scores[pos]); }, pos);

					//libraries
					models.resize(in_library::libs);

					for (auto& it : models)
					{
						{
							//first library line
							getline(in, line);

							pos = 0;
							read(line, [&]() ->unsigned int& { return (!pos ? it.booksnum : !(pos - 1) ? it.signup : it.ship); }, pos);
						}
						{
							//library list of books
							getline(in, line);
							it.books.resize(it.booksnum, 0);

							pos = 0;
							read(line, [&]() ->unsigned int& { return (it.books[pos]); }, pos);
						}
					}
				}
				else throw std::runtime_error("The number of libraries exceeds the total number of libraries available");
			}
			catch (std::runtime_error e)
			{
				throw e;
			}
			catch (...)
			{
				throw std::runtime_error("Please do not edit .hashcode data files");
			}


			//calculating the score
			std::clog << "Calculating the score..." << std::endl;

			unsigned int score = 0;

			{
				std::vector<unsigned int> readbooks;

				unsigned int days = 0;

				//analysing libraries
				for (const auto& out : libraries)
				{
					if (out.code >= models.size()) throw std::runtime_error("Invalid library ID: " + std::to_string(out.code));

					in_library& in = models[out.code];

					if (in.booksnum < out.booksnum) throw std::runtime_error("The number of books for library " + std::to_string(out.code) + " exceeds the total number of books available for this library");

					if ((days += in.signup) > in_library::days) break;

					for (unsigned int book = 0; book / in.ship < in_library::days - days && book < out.booksnum; book++)
					{
						bool ok = false;
						unsigned int code = out.books[book];

						for (auto it : in.books)
						{
							if (it == code)
							{
								ok = true;
								break;
							}
						}
						if (!ok) throw std::runtime_error("The book " + std::to_string(code) + " is not available at library " + std::to_string(out.code));
						for (auto it : readbooks)
						{
							if (it != code) continue;
							ok = false;
						}
						if (ok)
						{
							readbooks.push_back(code);
							score += in.scores[code];
						}
					}
				}
			}
			std::cout << std::endl << "Your score: " << score;


			//print files
			std::cout << "\n\nDo you want to print your file? (enter to skip)";

			std::string choose;
			getline(std::cin, choose);
			if (choose[0] != '\0') printin(libraries);

			std::cout << "\n\nDo you want to print google input dataset? (enter to skip)";

			getline(std::cin, choose);
			if (choose[0] != '\0')
			{
				printout(models);
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	//previous getchar()
			}
		}
		else throw std::runtime_error("File missing\nPut in this folder your submission named as Google's input file (e.g. " + subpath + ")");


		//end of the file
		std::clog << "Closing...";

		close(sub, in);
		return EXIT_SUCCESS;
	}
	catch (std::runtime_error e)
	{
		std::cerr << std::endl << e.what();
		(void)getchar();
		close(sub, in);

		return EXIT_FAILURE;
	}
}

int main()
{
	std::clog << "Hash Code 2020 Score Calculator By Foxel\n           Mono-File Version            \n\n";

	std::ifstream sub, in;

	if (calculate(sub, "a_example.txt", in, "data/a_example.hashcode"))
		if (calculate(sub, "b_read_on.txt", in, "data/b_read_on.hashcode"))
			if (calculate(sub, "c_incunabula.txt", in, "data/c_incunabula.hashcode"))
				if (calculate(sub, "d_tough_choices.txt", in, "data/d_tough_choices.hashcode"))
					if (calculate(sub, "e_so_many_books.txt", in, "data/e_so_many_books.hashcode"))
						if (calculate(sub, "f_libraries_of_the_world.txt", in, "f_libraries_of_the_world.hashcode"))
							return EXIT_SUCCESS;
	return EXIT_FAILURE;
}