#include <fstream>
#include <set>
#include "trietree.h"

//Распечатать все лозы
//Посчитать слова
//Построить копию

//Посчитать количество слов, которые начинаются с заданной подстроки
//Удалить все слова, начинающиеся с заданного фрагмента
//Построить копию-фрагмент дерева, начинающейся с заданной подстроки

void print_all(TrieTree& t, std::string word = "")
{
	if (all_ptrs_empty(t))
		std::cout << word << '\n';
	else
		for (size_t i = 0; i < 26; ++i)
			if (t->ptrs[i])
				print_all(t->ptrs[i], word + char(i + 'a'));
}

void count_words(TrieTree& t, int& count)
{
	if (t->eow)
		++count;
	for (size_t i = 0; i < 26; ++i)
		if (t->ptrs[i])
			count_words(t->ptrs[i], count);
}

int count_words(TrieTree& t)
{
	int count = 0;
	if (t->eow)
		count = 1;
	for (size_t i = 0; i < 26; ++i)
		if (t->ptrs[i])
			count += count_words(t->ptrs[i]);
	return count;
}

void copy(TrieTree& t, TrieTree& tc)
{
	if (t)
	{
		tc = new NODE(t->eow);
		for (size_t i = 0; i < 26; ++i)
			if (t->ptrs[i])
				copy(t->ptrs[i], tc->ptrs[i]);
	}
}

TrieTree copy(TrieTree& t)
{
	TrieTree tc = nullptr;
	if (t)
	{
		for (size_t i = 0; i < 26; ++i)
			if (t->ptrs[i])
				tc->ptrs[i] = copy(t->ptrs[i]);
	}
	return tc;
}

int count_sub(TrieTree& t, std::string sub, int i)
{
	int count = 0;
	if (sub.length() < i + 1)
		count = count_words(t);
	else
		if (t->ptrs[sub[i] - 'a'])
			count = count_sub(t->ptrs[sub[i] - 'a'], sub, i + 1);
	return count;
}

void clear_sub(TrieTree& t, std::string sub, int i)
{
	if (sub.length() < i + 1)
		clear(t);
	else
	{
		if (t->ptrs[sub[i] - 'a'])
			clear_sub(t->ptrs[sub[i] - 'a'], sub, i + 1);
		if (!t->eow && all_ptrs_empty(t))
		{
			delete t;
			t = nullptr;
		}
	}
}

TrieTree copy_sub(TrieTree& t, std::string sub, int i = 0)
{
	TrieTree result = nullptr;
	if (t)
	{
		if (sub.length() < i + 1)
			result = copy(t);
		else
		{
			if (t->ptrs[sub[i] - 'a'])
				result = copy_sub(t->ptrs[sub[i] - 'a'], sub, i + 1);
			TrieTree pr = new NODE;
			pr->ptrs[sub[i] - 'a'] = result;
			result = pr;
		}
	}
	return result;
}

void task(TrieTree& t, int is_vowels = 0, std::string word = "")
{
	if (is_vowels != -1)
	{
		std::set<char> vowels = { 'a', 'e', 'o', 'i', 'u',  'y' };

		if (t->eow)
			std::cout << word << '\n';

		for (size_t i = 0; i < 26; ++i)
			if (t->ptrs[i])
				task(t->ptrs[i], vowels.count(char(i + 'a')) ? 0 : -1, word + char(i + 'a'));
	}
}

int main()
{
	std::ifstream file("text.txt");
	TrieTree root;
	init(root);
	std::string word;
	while (std::getline(file, word))
		if (word.length())
			add(root, word, 0);
	file.close();
	print(root, "");
	std::cout << "------------\n";
	task(root);
	std::string subword;
	std::cin >> subword;
	std::cin.ignore();
	TrieTree cpyy;
	cpyy = copy_sub(root, subword);
	print(cpyy, "");
	clear(cpyy);
	clear(root);
	return 0;
}