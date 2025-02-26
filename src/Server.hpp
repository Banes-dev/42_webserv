#pragma once

#include <iostream>  	// base
#include <cstdlib>		// atof
#include <fstream>		// file gestion
#include <sstream>		// text gestion
#include <vector> 		// vector
#include <deque> 		// deque
#include <algorithm> 	// algo
#include <ctime>		// time

#include "Utils.hpp"


class Server
{
	public:
		Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);
		~Server();

		// Other function
		static std::vector<size_t> ConvertVector(char **argv);
		static std::deque<size_t> ConvertDeque(char **argv);
		
		static const std::vector<size_t> VectorTime(std::vector<size_t> list, double &time);
		static const std::deque<size_t> DequeTime(std::deque<size_t> list, double &time);
		
		static void PrintVector(const std::vector<size_t> &list);
		static void PrintDeque(const std::deque<size_t> &list);

		// Utils function
		static std::vector<size_t> VectorSort(std::vector<size_t> list);
		static std::deque<size_t> DequeSort(std::deque<size_t> list);

		static bool VectorIsSorted(std::vector<size_t> &list);
		static bool DequeIsSorted(std::deque<size_t> &list);

		// Exceptions
		class InvalidArgsException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mInvalid argument\033[0m");
				}
		};
		class NegativeArgsException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mNegative number not accepted\033[0m");
				}
		};
};
