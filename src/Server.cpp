#include "Server.hpp"


// Constructor & Destructor
Server::Server(void)
{
}

Server::Server(const Server &copy)
{
	*this = copy;
}

Server &Server::operator=(const Server &copy)
{
	(void)copy;
	return (*this);
}

Server::~Server(void)
{
}


// Other function
std::vector<size_t> Server::ConvertVector(char **argv)
{
	std::vector<size_t> list;
	int i = 0;
	while (argv[i])
	{
		if (std::atof(argv[i]) < 0)
			throw NegativeArgsException();
		std::stringstream ss(argv[i]);
		size_t num;
		ss >> num;
		if (ss.fail())
			throw InvalidArgsException();
		list.push_back(num);
		i++;
	}
	return (list);
}
std::deque<size_t> Server::ConvertDeque(char **argv)
{
	std::deque<size_t> list;
	int i = 0;
	while (argv[i])
	{
		if (std::atof(argv[i]) < 0)
			throw NegativeArgsException();
		std::stringstream ss(argv[i]);
		size_t num;
		ss >> num;
		if (ss.fail())
			throw InvalidArgsException();
		list.push_back(num);
		i++;
	}
	return (list);
}

const std::vector<size_t> Server::VectorTime(std::vector<size_t> list, double &time)
{
	std::clock_t start = std::clock();
	list = VectorSort(list);
	std::clock_t end = std::clock();
	time = ((end - start) * 1000000) / CLOCKS_PER_SEC;
	return (list);
}
const std::deque<size_t> Server::DequeTime(std::deque<size_t> list, double &time)
{
	std::clock_t start = std::clock();
	list = DequeSort(list);
	std::clock_t end = std::clock();
	time = ((end - start) * 1000000) / CLOCKS_PER_SEC;
	return (list);
}

void Server::PrintVector(const std::vector<size_t> &list)
{
	std::vector<size_t>::const_iterator it = list.begin();
	while (it != list.end())
	{
		std::cout << *it << " ";
		it++;
	}
}
void Server::PrintDeque(const std::deque<size_t> &list)
{
	std::deque<size_t>::const_iterator it = list.begin();
	while (it != list.end())
	{
		std::cout << *it << " ";
		it++;
	}
}

// Utils function
/////////////////////////// Vector ///////////////////////////
// Générer la suite de Jacobsthal jusqu'à une limite donnée
std::vector<size_t> VectorGenerateJacobsthalSequence(int n)
{
    std::vector<size_t> jacobsthal;
    jacobsthal.push_back(0);
    jacobsthal.push_back(1);

    while ((int)jacobsthal.size() < n)
	{
        int next = jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2];
        jacobsthal.push_back(next);
    }
    return (jacobsthal);
}
// Tri par insertion
void VectorInsertionSort(std::vector<size_t> &arr)
{
    for (size_t i = 1; i < arr.size(); i++)
	{
        size_t key = arr[i];
        size_t j = i;
        
        // Décaler les éléments plus grands vers la droite
        while (j > 0 && arr[j - 1] > key)
		{
            arr[j] = arr[j - 1];
            j--;
        }

        // Insérer l'élément à la bonne place
        arr[j] = key;
    }
}
// Recherche binaire
void VectorBinaryInsert(std::vector<size_t> &sortedList, int element)
{
    std::vector<size_t>::iterator it = std::lower_bound(sortedList.begin(), sortedList.end(), element);
    sortedList.insert(it, element);
}
std::vector<size_t> Server::VectorSort(std::vector<size_t> list)
{
	if (VectorIsSorted(list) == true)
		return (list);

    // Étape 1 : Créer des paires triées
    std::vector<std::pair<size_t, size_t> > pairs;
    std::vector<size_t> remaining;
    
    for (size_t i = 0; i + 1 < list.size(); i += 2)
	{
        if (list[i] > list[i + 1]) std::swap(list[i], list[i + 1]);
        pairs.push_back(std::make_pair(list[i], list[i + 1]));
    }

    // Ajouter l'élément restant s'il y a un nombre impair d'éléments
    if (list.size() % 2 == 1) remaining.push_back(list.back());

    // Étape 2 : Trier les petits éléments des paires
    std::vector<size_t> sortedList;
    for (size_t i = 0; i < pairs.size(); i++)
		sortedList.push_back(pairs[i].first);

	VectorInsertionSort(sortedList);
	// PrintVector(sortedList); std::cout << std::endl;

    // Étape 3 : Insérer les grands éléments des paires avec Jacobsthal
    std::vector<size_t> jacobsthal = VectorGenerateJacobsthalSequence(pairs.size());

    for (size_t i = 1; i < jacobsthal.size() && i - 1 < pairs.size(); i++)
	{
        size_t idx = jacobsthal[i] - 1; // La suite de Jacobsthal commence à 1 en général
        if (idx >= pairs.size()) break;
        VectorBinaryInsert(sortedList, pairs[idx].second);
    }
	// Vérifier que tous les grands éléments des paires ont été insérés
	for (size_t i = 0; i < pairs.size(); i++)
	{
		if (std::find(sortedList.begin(), sortedList.end(), pairs[i].second) == sortedList.end())
			VectorBinaryInsert(sortedList, pairs[i].second);
	}

    // Étape 4 : Insérer l’élément restant

    for (size_t i = 0; i < remaining.size(); i++)
        VectorBinaryInsert(sortedList, remaining[i]);

    return (sortedList);
}

/////////////////////////// Deque ///////////////////////////
// Générer la suite de Jacobsthal jusqu'à une limite donnée
std::deque<size_t> DequeGenerateJacobsthalSequence(int n)
{
    std::deque<size_t> jacobsthal;
    jacobsthal.push_back(0);
    jacobsthal.push_back(1);

    while ((int)jacobsthal.size() < n)
	{
        int next = jacobsthal[jacobsthal.size() - 1] + 2 * jacobsthal[jacobsthal.size() - 2];
        jacobsthal.push_back(next);
    }
    return (jacobsthal);
}
// Tri par insertion
void DequeInsertionSort(std::deque<size_t> &arr)
{
    for (size_t i = 1; i < arr.size(); i++)
	{
        size_t key = arr[i];
        size_t j = i;
        
        // Décaler les éléments plus grands vers la droite
        while (j > 0 && arr[j - 1] > key)
		{
            arr[j] = arr[j - 1];
            j--;
        }

        // Insérer l'élément à la bonne place
        arr[j] = key;
    }
}
// Recherche binaire
void DequeBinaryInsert(std::deque<size_t> &sortedList, int element)
{
    std::deque<size_t>::iterator it = std::lower_bound(sortedList.begin(), sortedList.end(), element);
    sortedList.insert(it, element);
}
std::deque<size_t> Server::DequeSort(std::deque<size_t> list)
{
	if (DequeIsSorted(list) == true)
		return (list);

    // Étape 1 : Créer des paires triées
    std::deque<std::pair<size_t, size_t> > pairs;
    std::deque<size_t> remaining;
    
    for (size_t i = 0; i + 1 < list.size(); i += 2)
	{
        if (list[i] > list[i + 1]) std::swap(list[i], list[i + 1]);
        pairs.push_back(std::make_pair(list[i], list[i + 1]));
    }

    // Ajouter l'élément restant s'il y a un nombre impair d'éléments
    if (list.size() % 2 == 1) remaining.push_back(list.back());

    // Étape 2 : Trier les petits éléments des paires
    std::deque<size_t> sortedList;
    for (size_t i = 0; i < pairs.size(); i++)
		sortedList.push_back(pairs[i].first);

	DequeInsertionSort(sortedList);

    // Étape 3 : Insérer les grands éléments des paires avec Jacobsthal
    std::deque<size_t> jacobsthal = DequeGenerateJacobsthalSequence(pairs.size());

    for (size_t i = 1; i < jacobsthal.size() && i - 1 < pairs.size(); i++)
	{
        size_t idx = jacobsthal[i] - 1; // La suite de Jacobsthal commence à 1 en général
        if (idx >= pairs.size()) break;
        DequeBinaryInsert(sortedList, pairs[idx].second);
    }
	// Vérifier que tous les grands éléments des paires ont été insérés
	for (size_t i = 0; i < pairs.size(); i++)
	{
		if (std::find(sortedList.begin(), sortedList.end(), pairs[i].second) == sortedList.end())
			DequeBinaryInsert(sortedList, pairs[i].second);
	}

    // Étape 4 : Insérer l’élément restant
    for (size_t i = 0; i < remaining.size(); i++)
        DequeBinaryInsert(sortedList, remaining[i]);

    return (sortedList);
}

bool Server::VectorIsSorted(std::vector<size_t> &list)
{
	if (list.size() <= 1)
		return (true);
	std::vector<size_t>::iterator it = list.begin();
	size_t prev = *it;
	++it;
	while (it != list.end())
	{
		if (*it < prev)
			return (false);
		prev = *it;
		++it;
	}
	return (true);
}
bool Server::DequeIsSorted(std::deque<size_t> &list)
{
	if (list.size() <= 1)
		return (true);
	std::deque<size_t>::iterator it = list.begin();
	size_t prev = *it;
	++it;
	while (it != list.end())
	{
		if (*it < prev)
			return (false);
		prev = *it;
		++it;
	}
	return (true);
}
