/*
Universitatea Tehnica a Moldovei
Lucrarea de laborator nr. 1 Programarea Calculatoarelor. Bazele limbajului C. 
Data 18.09.2021
Elaborat: Cernega Vladislav
Verificat: Dumitru Prijilevschi 
*/

#include <iostream>
#include <stack>
#include <math.h>
using namespace std;

const double Pi = acos(-1); //Declaram constanta Pi

double Sin(double x) { //Functia sinus cu rotungire
	return (round(sin(x) * 10000000) / 10000000);
}

double Cos(double x) { //Functia cosinus cu rotungire
	return (round(cos(x) * 10000000) / 10000000);
}

double ctg(double x) { //Functia de calcul a
	double a = Cos(x);
	double b = Sin(x);
	return (a / b);
}

struct Digit //
{
	char type; // 0 pentru cifre, +-/* pentru operatii
	double value; //Valoarea cifrei
};

bool Maths(stack <Digit>& Stack_n, stack <Digit>& Stack_o, Digit& item) { //Functia matematica care executa calculul expresiei
	//Functia are tipul bool, pentru a returna false in cazul unei erori
	double a, b, c;
	a = Stack_n.top().value;
	Stack_n.pop(); 
	switch (Stack_o.top().type) { 
	case '+': 
		b = Stack_n.top().value;
		Stack_n.pop();
		c = a + b;
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case '-':
		b = Stack_n.top().value;
		Stack_n.pop();
		c = b - a;
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case '^':
		b = Stack_n.top().value;
		Stack_n.pop();
		c = pow(b, a);
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case '*':
		b = Stack_n.top().value;
		Stack_n.pop();
		c = a * b;
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case '/':
		b = Stack_n.top().value;
		if (a == 0) {
			cerr << "\nError! Impartirea la 0 nu exista\n";
			return false;
		}
		else {
			Stack_n.pop();
			c = (b / a);
			item.type = '0';
			item.value = c;
			Stack_n.push(item);
			Stack_o.pop();
			break;
		}

	case 's':
		c = Sin(a);
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case 'c':
		c = Cos(a);
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case 't':
		if (Cos(a) == 0) {
			cerr << "\nArgument gresit pentru functia tangenta!\n";
			return false;
		}
		else {
			c = tan(a);
			item.type = '0';
			item.value = c;
			Stack_n.push(item);
			Stack_o.pop();
			break;
		}

	case 'g':
		if (Sin(a) == 0) {
			cerr << "\nArgument gresit pentru functia cotangenta!\n";
			return false;
		}
		else {
			c = ctg(a);
			item.type = '0';
			item.value = c;
			Stack_n.push(item);
			Stack_o.pop();
			break;
		}

	case 'e':
		c = exp(a);
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	case 'a':
		c = abs(a);
		item.type = '0';
		item.value = c;
		Stack_n.push(item);
		Stack_o.pop();
		break;

	default:
		cerr << "\nEroare!\n";
		return false;
		break;
	}
	return true;
}

int getPriority(char Ch) { //Functia returneaza prioritatea operatiei matematice
	if (Ch == '+' || Ch == '-')return 1;
	if (Ch == '*' || Ch == '/')return 2;
	if (Ch == '^')return 3;	
	if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'g' || Ch == 'e' || Ch == 'a')return 4;
	else return 0;
}

int main()
{
	cout << "   Pentru cifra pi introduceti 'p', pentru a folosi cifra e utilizati 'exp(1)'\n";
	cout << "   Introduceti expresia: ";
	char Ch; //Variabila care pastreaza caracterul care se prelucreaza
	double value;
	bool flag = 1; //Flag control pentru minus la inceput de expresie
	stack <Digit> Stack_n; //Stack cu cifre
	stack <Digit> Stack_o; //Stack cu operatii matematice
	Digit item; 
	while (1) {
		Ch = cin.peek(); //Peek primul caracter din expresie
		if (Ch == '\n')break; //Iesirea din ciclu la sfarsit de rand
		if (Ch == ' ') { //Ignorarea spatiilor
			cin.ignore();
			continue;
		}
		if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'e' || Ch == 'a') { 
			char funcs[3]; //Tabel din 3 caractere pentru determinarea functiei utilizate
			for (int i = 0; i < 3; i++) {
				Ch = cin.peek();
				funcs[i] = Ch;
				cin.ignore();
				flag = 1;
			}
			if (funcs[0] == 's' && funcs[1] == 'i' && funcs[2] == 'n') {
				item.type = 's';
				item.value = 0;
				Stack_o.push(item); 
				continue;
			}
			if (funcs[0] == 'c' && funcs[1] == 'o' && funcs[2] == 's') { 
				item.type = 'c';
				item.value = 0;
				Stack_o.push(item); 
				continue;
			}
			if (funcs[0] == 't' && funcs[1] == 'a' && funcs[2] == 'n') { 
				item.type = 't';
				item.value = 0;
				Stack_o.push(item);
				continue;
			}
			if (funcs[0] == 'c' && funcs[1] == 't' && funcs[2] == 'g') { 
				item.type = 'g';
				item.value = 0;
				Stack_o.push(item); 
				continue;
			}
			if (funcs[0] == 'e' && funcs[1] == 'x' && funcs[2] == 'p') { 
				item.type = 'e';
				item.value = 0;
				Stack_o.push(item); 
				continue;
			}
			if (funcs[0] == 'a' && funcs[1] == 'b' && funcs[2] == 's') { 
				item.type = 'a';
				item.value = 0;
				Stack_o.push(item); 
				continue;
			}
			flag=0;
		}
		if (Ch == 'p') { 
			item.type = '0';
			item.value = Pi;
			Stack_n.push(item); 
			flag = 0;
			cin.ignore();
			continue;
		}
		if ((Ch >= '0' && Ch <= '9') || (Ch == '-' && flag == 1)) { 
			cin >> value;
			item.type = '0';
			item.value = value;
			Stack_n.push(item);
			flag = 0;
			continue;
		}
		if (Ch == '+' || (Ch == '-' && flag == 0) || Ch == '*' || Ch == '/' || Ch == '^') { 
			if (Stack_o.size() == 0) { 
				item.type = Ch;
				item.value = 0;
				Stack_o.push(item);
				cin.ignore();
				continue;
			}
			if (Stack_o.size() != 0 && getPriority(Ch) > getPriority(Stack_o.top().type)) { 
				item.type = Ch;
				item.value = 0;
				Stack_o.push(item); 
				cin.ignore();
				continue;
			}
			if (Stack_o.size() != 0 && getPriority(Ch) <= getPriority(Stack_o.top().type)) {
				if (Maths(Stack_n, Stack_o, item) == false) { 
					system("pause");
					return 0;
				}
				continue;
			}
		}
		if (Ch == '(') {
			item.type = Ch;
			item.value = 0;
			Stack_o.push(item);
			cin.ignore();
			continue;
		}
		if (Ch == ')') { 
			while (Stack_o.top().type != '(') {
				if (Maths(Stack_n, Stack_o, item) == false) { 
					system("pause");
					return 0;
				}
				else continue; 
			}
			Stack_o.pop();
			cin.ignore();
			continue;
		}
		else { 
			cout << "\nExpresia a fost introdusa gresit!\n";
			system("pause");
			return 0;
		}
	}
	while (Stack_o.size() != 0) { 
		if (Maths(Stack_n, Stack_o, item) == false) { 
			system("pause");
			return 0;
		}
		else continue; 
	}
	cout << "   Raspuns: " << Stack_n.top().value << endl; //Afisarea raspunsului final
	return 0;
}
