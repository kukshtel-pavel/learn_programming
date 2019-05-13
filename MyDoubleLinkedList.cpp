#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

struct Node
{
	int date;							//значение элемента
	Node *pLeft;						//указатель на соседний (левый/предыдущий элемент)
	Node *pRight;						//указатель на соседний (правый/следующий элемент)
};

class List
{
private:
	int count;							//счетчик элементов списка
	Node *pTail;						//конец списка / хвост
	Node *pHead;						//начало списка / голова
public:
	List();								//консруктор
	List(int x);						//консруктор с параметрами (параметризованный конструктор) 
	List(const List &obj);				//конструктор копирования
	~List();							//деструктор

	Node* operator[](int numElem);		//перегрузка оператора '[]'
	void showList();					//выводим лист в консоль
	void pushBack();					//добавляем рандомный елемент в конец списка
	void pushBack(int x);				//добавляем елемент = 'X' в конец списка
	void pushFront();					//добавляем рандомный елемент в начало списка
	Node* goTo(int pos);				//возвращаем указатель на элемент под номером "pos" (для служебных целей)
	int tellPos(Node* pos);				//возвращаем порядковый номер элемента по указателю (для служебных целей)
	void delFirst();					//удаляем первый элемент списка
	void delLast();						//удаляем последний элемент списка
	void rmFrPos(int x);				//удаляем элемент с заданной позиции списка
	void printEl(int pos);				//возвращаем элемент с заданной позиции
	Node* searchMax();					//находим указатель на максимальный элемент
	Node* searchMin();					//находим указатель на минимальный элемент
	bool searchVal(int val);			//проверка на наличие значения в элементах списка
	void addTo(int x);					//добавить элемент в заданную позицию
	void sort();						//отсортировать элементы в списке по возрастанию
	void toFile();						//записать список в текстовый файлу
	void setDate(int val, int pos);		//установить значение элемента "val" на позицию "pos"
	void getCount();					//узнаем количество элементов в списке
};

List::List()							
{
	Node *temp = new Node;
	temp->pLeft = nullptr;
	pTail = temp;
	pHead = temp;
	temp->pRight = nullptr;
	temp->date = rand() % 1000;
	count = 1;
}					
List::List(int x)
{
	Node *temp = new Node;
	temp->pLeft = nullptr;
	pTail = temp;
	pHead = temp;
	temp->pRight = nullptr;
	temp->date = x;
	count = 1;
}
List::~List()
{
	while (count > 0)
	{
		Node *temp = pTail;
		if (count > 1)
		{
			temp->pLeft->pRight = nullptr;
			pTail = temp->pLeft;
		}
		count--;

		delete temp;
	};

}
List::List(const List &obj)			//Конструктор копирования
{
	Node *temp = new Node;
	temp->pLeft = nullptr;
	pTail = temp;
	pHead = temp;
	temp->pRight = nullptr;
	count = 1;
	temp->date = obj.pHead->date;
	
	Node *etalon = obj.pHead->pRight;

	while (etalon->pRight != nullptr)
	{
		pushBack(etalon->date);
		etalon = etalon->pRight;
	}
	pushBack(etalon->date);
}

void List::showList()
{
	if (count <= 0) { cout << "YOUR list is empty" << endl; }
	else {
		Node *temp = pHead;
		while (temp->pRight != nullptr)
		{
			cout << "Value\t" << temp->date << "\taddres of Val\t" << temp << endl;
			temp = temp->pRight;
		}
		cout << "Value\t" << temp->date << "\taddres of Val\t" << temp << endl;
	}
}
void List::pushBack()
{
	Node *temp = new Node;
	temp->date = rand() % 1000;
	pTail->pRight = temp;				//сообщаем предыдущему элементу о соседе справа
	temp->pLeft = pTail;
	pTail = temp;						//теперь хвост списка - это новый элемент
	temp->pRight = nullptr;				//справа пусто
	count++;
}
void List::pushBack(int x)
{
	Node *temp = new Node;
	temp->date = x;
	pTail->pRight = temp;				//сообщаем предыдущему элементу о соседе справа
	temp->pLeft = pTail;
	pTail = temp;						//теперь хвост списка - это новый элемент
	temp->pRight = nullptr;				//справа пусто
	count++;
}
void List::pushFront()
{
	Node *temp = new Node;
	temp->date = rand() % 1000;
	pHead->pLeft = temp;
	temp->pRight = pHead;				//сообщаем следующему элементу о соседе слева
	pHead = temp;						//теперь голова списка - это новый элемент
	temp->pLeft = nullptr;				//слева пусто	
	count++;
}
void List::addTo(int pos)
{
	if (pos > count || pos < 0)					//когда желаемая позиция вне пределов списка
	{
		cout << endl << "ATTENTION !!! You are left the border !!!" << endl;
	}
	else if (pos == 0)							//когда вставляем в начале списка
	{
		pushFront();
	}
	else if (count == pos)						//когда вставляем в конец списка
	{
		pushBack();
	}
	else                                        //когда вставка внутри списка
	{
		Node *pTempLeft;				//создаем служебный указатель, буде левее нового
		pTempLeft = goTo(pos - 1);
		Node *pTempRight;				//создаем служебный указатель, буде правее нового
		pTempRight = goTo(pos);

		Node *temp = new Node;
		temp->date = rand() % 1000;
		pTempLeft->pRight = pTempRight;			//соседу слева меняем левый элемент на новый
		pTempLeft->pRight = temp;
		pTempRight->pLeft = temp;				//соседу справа меняем левый элемент на новый

		temp->pLeft = pTempLeft;				//указываем новому элементу соседа слева
		temp->pRight = pTempRight;				//указываем новому элементу соседа справа
		count++;

	}
}

void List::sort()
{
	int replace;
	int min = pHead->date;
	Node *temp = pHead;
	Node *tempHead = pHead;
	Node *pMin = pHead;
	while (tempHead->pRight != nullptr)
	{
		while (temp->pRight != NULL)
		{
			temp = temp->pRight;
			if (min > (temp->date)) { min = temp->date; pMin = temp; };
		}

		replace = tempHead->date;				//меняем местами мин значение с первым
		tempHead->date = min;
		pMin->date = replace;

		tempHead = tempHead->pRight;
		pMin = tempHead;
		min = tempHead->date;
		temp = tempHead;
	}
}

Node * List::goTo(int numEl)
{
	if (numEl >= count || numEl < 0)
	{
		cout << "WARNING!!!! You are left the border" << endl;
		return nullptr;
	}
	else
	{
		int tmpCount = 0;
		Node *temp = pHead;
		while (tmpCount < numEl)
		{
			temp = temp->pRight;
			tmpCount++;
		}
		return temp;
	};
}
int List::tellPos(Node* posEl)
{
	int rez = 0;
	Node *temp = pHead;
	while (posEl != temp)
	{
		temp = temp->pRight;
		rez++;
	}
	cout << "Element position " << rez << endl;
	return rez;
}
Node* List::operator[](int numEl)		//Отсчет идет с 0 элемента
{
	if (numEl >= count)
	{
		// You are left the border 
		return NULL;
	}
	else
	{
		int tmpCount = 0;
		Node *temp = pHead;
		while (tmpCount < numEl)
		{
			temp = temp->pRight;
			tmpCount++;
		}
		return temp;
	}
}
void List::printEl(int pos)
{
	if (pos >= count || pos < 0)
	{
		cout << "WARNING!!!! You are left the border" << endl;
	}
	else
	{
		Node *temp;
		temp = goTo(pos);
		cout << temp->date << endl;
	}
}

void List::delFirst()
{
	Node *temp = pHead;
	count--;
	if (count > 0)
	{
		pHead = temp->pRight;					//Перезаписываем голову, если еще остались элементы
		pHead->pLeft = NULL;
	}
	delete temp;
}
void List::delLast()
{
	Node *temp = pTail;
	if (count > 1) {
		pTail = temp->pLeft;
		pTail->pRight = NULL;
	}
	count--;
	delete temp;
}
void List::rmFrPos(int pos)
{

	if (pos >= count || pos < 0)					//когда желаемая позиция вне пределов списка
	{
		cout << endl << "ATTENTION !!! You are left the border !!!" << endl;
	}
	else if (count == 0)							//когда список пуст
	{
		cout << "your LIST is empty" << endl;

	}
	else if (count == (pos + 1) && pos != 0)				//когда удаляем в конце списка
	{
		delLast();
	}
	else if (pos == 0)								//когда удаляем в начале списка
	{
		delFirst();
	}
	else                                            //все остальные случаи
	{
		Node *temp = pHead;
		/*int tempCount = 0;
		while (tempCount != pos)
		{
			temp = temp->pRight;
			tempCount++;
		}*/
		temp = goTo(pos);
		temp->pLeft->pRight = temp->pRight;
		temp->pRight->pLeft = temp->pLeft;
		count--;
		delete temp;
	}
}

Node * List::searchMax()
{
	int max = pHead->date;
	Node *temp = pHead;
	Node *pMax = pHead;
	while (temp->pRight != NULL)
	{
		temp = temp->pRight;
		if (max < (temp->date)) { max = temp->date; pMax = temp; };

	}
	cout << "Max val = " << pMax->date << "\t" << pMax << endl;
	return pMax;
}
Node * List::searchMin()
{
	int min = pHead->date;
	Node *temp = pHead;
	Node *pMin = pHead;
	while (temp->pRight != NULL)
	{
		temp = temp->pRight;
		if (min > (temp->date)) { min = temp->date; pMin = temp; };

	}
	cout << "Min val = " << pMin->date << "\t" << pMin << endl;
	return pMin;
}
bool List::searchVal(int val)
{
	Node* temp;
	temp = pHead;
	bool status = false;
	while (temp->pRight != nullptr)
	{
		if (temp->date == val)
		{
			status = true;
		}
		temp = temp->pRight;
	}
	if (temp->date == val) { status = true; }				//проверка последнего элемента


	if (status) { cout << "We have element \"" << val << "\" in this LIST" << endl; return status; }
	else { cout << "We don't have element " << val << "in this LIST" << endl; return status; }

}
void List::toFile()
{
	ofstream fout("LIST.txt");
	if (fout.is_open())
	{
		if (count <= 0) { fout << "YOUR list is empty" << endl; }
		else {
			Node *temp = pHead;
			while (temp->pRight != nullptr)
			{
				fout << "Value\t" << temp->date << "\taddres of Val\t" << temp << endl;
				temp = temp->pRight;
			}
			fout << "Value\t" << temp->date << "\taddres of Val\t" << temp << endl;
		}
	}
	fout.close();
}
void List::getCount()
{
	cout <<"Count of this LIST = " << this->count << endl;
}
void List::setDate(int val, int pos)
{
	Node* temp;
	temp = goTo(pos);
	temp->date = val;
}


int main()
{
	Node *brakets;
	srand(time(NULL));
	cout << "List 1 " << endl;
	List lst(77);
	lst.pushBack();
	lst.pushBack(3);
	lst.pushBack();
	lst.pushFront();
	lst.pushBack();
	lst.pushBack(2);
	
	lst.showList();
	lst.getCount();
	cout << endl;
	brakets = lst[3];
	cout <<"operator '[]' return addres of 3-elem " << brakets << endl;
	cout << "remove 3rd elem " << endl;
	lst.rmFrPos(3);
	cout << "Date on 3rd position " ;
	lst.printEl(3);
	cout << endl;
	lst.getCount();
	cout << endl;

	cout << "List 2 " << endl;
	List lst2(lst);		
	lst2.showList();
	lst2.getCount();
	lst2.toFile();
	cout << endl;

	cout << "List 1 " << endl;
	lst.setDate(200, 2);
	lst.sort();
	lst.showList();
	lst.searchVal(77); 
	system("pause");
	cout << endl;
};
