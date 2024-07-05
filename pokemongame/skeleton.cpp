//////////////////////////////////////////
// The code from Zeynep Gurler's HW is altered for the application. Sincere thanks!
//////////////////////////////////////////
#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include "doublelinklist.h"

using namespace std;

struct attack
{ // class for pokemon attacks
private:
	string name;
	int pp;
	int accuracy;
	int damage;
	int first;

public:
	attack(const string &, int, int, int, int);
	int get_first();
	int get_accuracy();
	int get_damage();
	int get_pp();
	string get_name();
};

attack::attack(const string &name_in, int p, int a, int d, int f)
{
	name = name_in;
	pp = p;
	accuracy = a;
	damage = d;
	first = f;
}

string attack::get_name()
{
	return name;
}

int attack::get_pp()
{
	return pp;
}

int attack::get_damage()
{
	return damage;
}

int attack::get_first()
{
	return first;
}

int attack::get_accuracy()
{
	return accuracy;
}

struct pokemon
{ // class for pokemons
	int pp;
	int hp;
	string name;
	DoublyList<attack *> attacks;
	pokemon(const string &name_in, int, int);
	string get_name();
	void set(char, int to_reduce);
};

pokemon::pokemon(const string &name_in, int p, int h)
{
	name = name_in;
	pp = p;
	hp = h;
}

void pokemon::set(char which, int to_reduce)
{
	if (which == 'p')
	{
		pp += to_reduce;
	}
	else
	{
		hp -= to_reduce;
	}
}

struct node
{						// class for the nodes of the graph
	string ifef;		// attack is efficient or nonefficent
	string curattack;	// attack name
	int num;			// node num
	pokemon *pikachu;	// current pikachu
	pokemon *blastoise; // currrent blastoise
	char status;		// whose turn?
	int level;			// level of the node
	bool isleaf;		// node is a leaf or not
	double prob;		// prob of the node happening

	DoublyList<node *> child; // children nodes of the node

	// Constructor
	node(string ifef, string curattack, int num, pokemon *pikachu, pokemon *blastoise, char status, int level, bool isleaf, double prob)
	{
		this->ifef = ifef;
		this->curattack = curattack;
		this->num = num;
		this->pikachu = pikachu;
		this->blastoise = blastoise;
		this->status = status;
		this->level = level;
		this->isleaf = isleaf;
		this->prob = prob;
	}
};
DoublyList<node *> minsize(DoublyList<node *> A, DoublyList<node *> B)
{
	if (A.elemcount < B.elemcount)
	{
		return A;
	}
	else
		return B;
}
struct graph
{
	node *root = nullptr;
	int elemcount = 1;
	void deletegraph(node *r);
	void AddLevel(node *N, int maxlvl);
	void buildgraph(pokemon *pik, pokemon *bla, int maxlvl);
	void PrintGraph(node *r, int lvl);

	void FindEasiestPath(string s);
	DoublyList<node *> minnode(node *r, string s, DoublyList<node *> A);
	bool Path(node *current, node *target, DoublyList<node *> &A);
};
void graph::deletegraph(node *r)
{
	if(r==nullptr){
		return;
	}

	for (int i = 0; i < r->child.elemcount; i++)
	{
		deletegraph(r->child.get(i));
	}
	delete r->pikachu;
	delete r->blastoise;
	delete r;
}


DoublyList<node *> graph::minnode(node *r, string s, DoublyList<node *> A)
{ // searching every nodes for given String and return appropriate nodes in list

	if (s == "pikachu")
	{
		if (r->blastoise->hp <= 0)
		{
			if (A.elemcount > 0)
			{

				if (A.tail->data->prob <= r->prob)
				{
					A.removeBack();
					A.addBack(r);
				}
			}
			else
			{
				A.addBack(r);
			}
			return A;
		}
		else
		{
			for (int i = 0; i < r->child.elemcount; i++)
			{
				A = minnode(r->child.get(i), s, A);
			}
		}
	}
	else
	{
		if (r->pikachu->hp <= 0)
		{
			if (A.elemcount > 0)
			{

				if (A.tail->data->prob <= r->prob)
				{
					A.removeBack();
					A.addBack(r);
				}
			}
			else
			{
				A.addBack(r);
			}
			return A;
		}
		else
		{
			for (int i = 0; i < r->child.elemcount; i++)
			{
				A = minnode(r->child.get(i), s, A);
			}
		}
	}
	return A;
}
bool graph::Path(node *current, node *target, DoublyList<node *> &A)
{ // finding path from root for given target and write it.

	if (current == target)
	{
		A.addFront(current);
		return true;
	}
	for (int i = 0; i < current->child.elemcount; ++i)
	{

		if (Path(current->child.get(i), target, A))
		{

			A.addFront(current);
			if (current == root)
			{
				for (int i = 0; i < A.elemcount; i++)
				{
					cout << A.get(i)->level << ": " << A.get(i)->ifef << " Prob: " << A.get(i)->prob << "\n";
				}
			}
			return true;
		}
	}

	return false;
}
void graph::PrintGraph(node *r, int lvl)
{ // searchin level and printing that level
	if (r->level == lvl)
	{

		cout << " P HP: " << r->pikachu->hp << " P PP: " << r->pikachu->pp << " B HP: " << r->blastoise->hp << " B PP: " << r->blastoise->pp << " PROB: " << r->prob << "\n";
		return;
	}
	for (int i = 0; i < r->child.elemcount; i++)
	{
		PrintGraph(r->child.get(i), lvl);
	}
}

void graph::AddLevel(node *N, int maxlvl)
{
	int elem = 0;
	if (N->isleaf)
	{
		return;
	}
	if (N->status == 'p')
	{
		for (int i = 0; i < N->pikachu->attacks.elemcount; i++)
		{
			if (N->pikachu->attacks.get(i)->get_first() <= N->level + 1 && N->pikachu->pp + N->pikachu->attacks.get(i)->get_pp() >= 0)
			{
				elem++;
			}
		}
		for (int i = 0; i < N->pikachu->attacks.elemcount; i++)
		{
			if (N->pikachu->attacks.get(i)->get_first() <= N->level + 1)
			{
				if (N->pikachu->pp + N->pikachu->attacks.get(i)->get_pp() >= 0)
				{
					if (N->pikachu->attacks.get(i)->get_accuracy() != 100)
					{

						bool leaf;
						double prob = N->prob / elem;
						pokemon *newpik = new pokemon(N->pikachu->name, N->pikachu->pp, N->pikachu->hp);
						newpik->attacks = N->pikachu->attacks;
						pokemon *newbla = new pokemon(N->blastoise->name, N->blastoise->pp, N->blastoise->hp);
						newbla->attacks = N->blastoise->attacks;
						newpik->set('p', N->pikachu->attacks.get(i)->get_pp());
						if (N->level + 1 == maxlvl)
						{
							leaf = true;
						}
						else
						{
							leaf = false;
						}
						elemcount += 1;
						node *newn = new node("Pikachu used " + N->pikachu->attacks.get(i)->get_name() + ": Not Effective", N->pikachu->attacks.get(i)->get_name(), elemcount, newpik, newbla, 'b', N->level + 1, leaf, prob - prob * N->pikachu->attacks.get(i)->get_accuracy() / 100);
						N->child.addBack(newn);

						pokemon *newpik2 = new pokemon(N->pikachu->name, N->pikachu->pp, N->pikachu->hp);
						newpik2->attacks = N->pikachu->attacks;
						pokemon *newbla2 = new pokemon(N->blastoise->name, N->blastoise->pp, N->blastoise->hp);
						newbla2->attacks = N->blastoise->attacks;
						newpik2->set('p', N->pikachu->attacks.get(i)->get_pp());
						newbla2->set('h', N->pikachu->attacks.get(i)->get_damage());
						if (newbla2->hp <= 0 || N->level + 1 == maxlvl)
						{
							leaf = true;
						}
						else
						{
							leaf = false;
						}
						elemcount += 1;
						node *newn2 = new node("Pikachu used " + N->pikachu->attacks.get(i)->get_name() + ": Effective", N->pikachu->attacks.get(i)->get_name(), elemcount, newpik2, newbla2, 'b', N->level + 1, leaf, prob * N->pikachu->attacks.get(i)->get_accuracy() / 100);
						N->child.addBack(newn2);
					}
					else
					{

						bool leaf;
						double prob = N->prob / elem;
						pokemon *newpik = new pokemon(N->pikachu->name, N->pikachu->pp, N->pikachu->hp);
						newpik->attacks = N->pikachu->attacks;
						pokemon *newbla = new pokemon(N->blastoise->name, N->blastoise->pp, N->blastoise->hp);
						newbla->attacks = N->blastoise->attacks;
						newpik->set('p', N->pikachu->attacks.get(i)->get_pp());
						newbla->set('h', N->pikachu->attacks.get(i)->get_damage());
						if (newbla->hp <= 0 || N->level + 1 == maxlvl)
						{
							leaf = true;
						}
						else
						{
							leaf = false;
						}
						elemcount += 1;
						node *newn = new node("Pikachu used " + N->pikachu->attacks.get(i)->get_name() + ": Effective", N->pikachu->attacks.get(i)->get_name(), elemcount, newpik, newbla, 'b', N->level + 1, leaf, prob);
						N->child.addBack(newn);
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < N->blastoise->attacks.elemcount; i++)
		{
			if (N->blastoise->attacks.get(i)->get_first() <= N->level + 1 && N->blastoise->pp + N->blastoise->attacks.get(i)->get_pp() >= 0)
			{
				elem++;
			}
		}
		for (int i = 0; i < N->blastoise->attacks.elemcount; i++)
		{
			if (N->blastoise->attacks.get(i)->get_first() <= N->level + 1)
			{

				if (N->blastoise->attacks.get(i)->get_accuracy() != 100)
				{
					if (N->blastoise->pp + N->blastoise->attacks.get(i)->get_pp() >= 0)
					{
						bool leaf;
						double prob = N->prob / elem;
						pokemon *newpik = new pokemon(N->pikachu->name, N->pikachu->pp, N->pikachu->hp);
						newpik->attacks = N->pikachu->attacks;
						pokemon *newbla = new pokemon(N->blastoise->name, N->blastoise->pp, N->blastoise->hp);
						newbla->attacks = N->blastoise->attacks;

						newbla->set('p', N->blastoise->attacks.get(i)->get_pp());

						if (N->level + 1 == maxlvl)
						{
							leaf = true;
						}
						else
						{
							leaf = false;
						}
						elemcount += 1;
						node *newn = new node("Blastoise used " + N->blastoise->attacks.get(i)->get_name() + ": Not Effective", N->blastoise->attacks.get(i)->get_name(), elemcount, newpik, newbla, 'p', N->level + 1, leaf, prob - prob * N->blastoise->attacks.get(i)->get_accuracy() / 100);
						N->child.addBack(newn);

						pokemon *newpik2 = new pokemon(N->pikachu->name, N->pikachu->pp, N->pikachu->hp);
						newpik2->attacks = N->pikachu->attacks;
						pokemon *newbla2 = new pokemon(N->blastoise->name, N->blastoise->pp, N->blastoise->hp);
						newbla2->attacks = N->blastoise->attacks;

						newbla2->set('p', N->blastoise->attacks.get(i)->get_pp());
						newpik2->set('h', N->pikachu->attacks.get(i)->get_damage());

						if (newpik2->hp <= 0 || N->level + 1 == maxlvl)
						{
							leaf = true;
						}
						else
						{
							leaf = false;
						}
						elemcount += 1;
						node *newn2 = new node("Blastoise used " + N->blastoise->attacks.get(i)->get_name() + ": Effective", N->blastoise->attacks.get(i)->get_name(), elemcount, newpik2, newbla2, 'p', N->level + 1, leaf, prob * N->blastoise->attacks.get(i)->get_accuracy() / 100);
						N->child.addBack(newn2);
					}
				}
				else
				{
					if (N->blastoise->pp + N->blastoise->attacks.get(i)->get_pp() >= 0)
					{
						bool leaf;
						double prob = N->prob / elem;

						pokemon *newpik = new pokemon(N->pikachu->name, N->pikachu->pp, N->pikachu->hp);
						newpik->attacks = N->pikachu->attacks;
						pokemon *newbla = new pokemon(N->blastoise->name, N->blastoise->pp, N->blastoise->hp);
						newbla->attacks = N->blastoise->attacks;

						newbla->set('p', N->blastoise->attacks.get(i)->get_pp());
						newpik->set('h', N->blastoise->attacks.get(i)->get_damage());
						if (newpik->hp <= 0 || N->level + 1 == maxlvl)
						{
							leaf = true;
						}
						else
						{
							leaf = false;
						}
						elemcount += 1;
						node *newn = new node("Blastoise used " + N->blastoise->attacks.get(i)->get_name() + ": Effective", N->blastoise->attacks.get(i)->get_name(), elemcount, newpik, newbla, 'p', N->level + 1, leaf, prob);
						N->child.addBack(newn);
					}
				}
			}
		}
	}
	for (int i = 0; i < N->child.elemcount; i++)
	{
		if (N->child.get(i)->isleaf)
		{
			return;
		}
		AddLevel(N->child.get(i), maxlvl); // adding new levels for every child
	}
}
void graph::buildgraph(pokemon *pik, pokemon *bla, int maxlvl)
{
	root = new node("", "", 0, pik, bla, 'p', 0, false, 1.0);
	if (maxlvl == 0)
	{
		root->isleaf = true;
		return;
	}
	AddLevel(root, maxlvl);
}
void graph::FindEasiestPath(string s)
{
	DoublyList<node *> A;
	A = minnode(root, s, A); // finding nodes that given pokemon wins
	node *temp = A.tail->data; // max probability node
	DoublyList<node *> B;
	Path(root, temp, B); // path from root to target
}

int main(int argc, char **argv)
{

	pokemon *pikachu = new pokemon("pikachu", 100, 200);
	pokemon *blastoise = new pokemon("blastoise", 100, 200);

	attack *thundershock = new attack("thundershock", -10, 100, 40, 0);
	attack *skullbash = new attack("skullbash", -15, 70, 50, 0);
	attack *slam = new attack("slam", -20, 80, 60, 0);
	attack *pskip = new attack("skip", 100, 100, 0, 3);
	pikachu->attacks.addBack(thundershock);
	pikachu->attacks.addBack(skullbash);
	pikachu->attacks.addBack(slam);
	pikachu->attacks.addBack(pskip);

	attack *tackle = new attack("tackle", -10, 100, 30, 0);
	attack *watergun = new attack("watergun", -20, 100, 40, 0);
	attack *bite = new attack("bite", -25, 100, 60, 0);
	attack *bskip = new attack("skip", 100, 100, 0, 3);
	blastoise->attacks.addBack(tackle);
	blastoise->attacks.addBack(watergun);
	blastoise->attacks.addBack(bite);
	blastoise->attacks.addBack(bskip);

	string part = argv[1];
	int max;
	string which;
	if (part == "part1")
	{
		which = argv[2];
		max = 10;
		graph g;
		g.buildgraph(pikachu, blastoise, max);
		g.PrintGraph(g.root, stoi(which));
		g.deletegraph(g.root);
	}
	else if (part == "part2")
	{
		which = argv[2];
		max = 10;
		graph g;
		g.buildgraph(pikachu, blastoise, max);
		g.FindEasiestPath(which);
		g.deletegraph(g.root);
	}
	else
		return -1;

	return 0;
}
