#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

class Employee {
    private:
        int id;
        int salary;
        int department;
        friend class TreeNode;
    public:
        Employee(int i,int s,int d){
        this->id = i;
        this->salary = s;
        this->department = d;
        }
        ~Employee(){}
        void set_id(int id){this->id = id;}
        void set_salary(int salary){this->salary = salary;}
        void set_department(int department){this->department = department;}
        int get_id(){return this->id;}
        int get_salary(){return this->salary;}
        int get_department(){ return this->department;}
};
class DoublySkipList_Node{
    private:
        Employee* data;
        DoublySkipList_Node* next;
        DoublySkipList_Node* below;
    public:
        DoublySkipList_Node(Employee *data){
            this->data = data;
            this->next = nullptr;
            this->below = nullptr;
        };
        ~DoublySkipList_Node(){};
        //getters and setters
        void set_data(Employee* data){this->data = data;}
        void set_next(DoublySkipList_Node* next){this->next = next;}
        void set_below(DoublySkipList_Node* below){this->below = below;}
        Employee* get_data(){return this->data;}
        DoublySkipList_Node* get_next(){return this->next;}
        DoublySkipList_Node* get_below(){return this->below;}
};
class DoublySkipList{
    private:
        DoublySkipList_Node* head;
        int height;
    public:
        DoublySkipList(int height_in){
            this->height = height_in;

            head = new DoublySkipList_Node(nullptr);

            DoublySkipList_Node* start = new DoublySkipList_Node(nullptr);
            DoublySkipList_Node* end = new DoublySkipList_Node(nullptr);
            head->set_next(start);
            start->set_next(end);
            DoublySkipList_Node* currstart = start;
            DoublySkipList_Node* currend = end;
            for(int i = 1; i <height;i++){ //Create starting and ending nodes equal to the height. head --> start nodes --> end nodes
                DoublySkipList_Node* start = new DoublySkipList_Node(nullptr);
                DoublySkipList_Node* end = new DoublySkipList_Node(nullptr);
                currstart->set_below(start);
                currend->set_below(end);
                currstart = start;
                currend = end;
                currstart->set_next(currend);
            }
        }
        ~DoublySkipList(){};
        void insert(Employee* data);
        void remove(int remove_id);
        Employee* search(int search_id);
        void dumpToFile(ofstream& out_file);
        void deletesl();
        void update(int,int,int);
};
void DoublySkipList::insert(Employee* data){ 
    /*The level and number of additions are kept.
    The inserting process starts when the add number is equal to the level 
    and it continues inserting by going down. */
    int number_of_add=1;
    while (number_of_add<height && rand()%2==1 ){
        number_of_add++;
    }
    int level = height;
    DoublySkipList_Node* newnode = new DoublySkipList_Node(data);
    DoublySkipList_Node* current = head->get_next();
    while(number_of_add!=0){
        if(current->get_next()->get_data()==nullptr){ // next node is end node.
            if(number_of_add==level){
                newnode->set_next(current->get_next());
                current->set_next(newnode);
                if(number_of_add!=1){ //if add not equal to 1, create a below node and set it. Also below node keeping as a newnode.
                    DoublySkipList_Node* newnode1 = new DoublySkipList_Node(data);
                    newnode->set_below(newnode1);
                    newnode = newnode1;
                }
                number_of_add--;
            }
            else{
                current = current->get_below(); // went below so level decreased by 1.
                level--;
            }
        }
        else if(current->get_next()->get_data()->get_id()>data->get_id() && number_of_add==level){
                newnode->set_next(current->get_next());
                current->set_next(newnode);
                if(number_of_add!=1){
                    DoublySkipList_Node* newnode1 = new DoublySkipList_Node(data);
                    newnode->set_below(newnode1);
                    newnode = newnode1;
                }
                number_of_add--; // One addition  has been done so add decreased
        }
        else if (current->get_next()->get_data()->get_id() < data->get_id()){
            current=current->get_next();
        }
        else{
            current=current->get_below(); // went below so level decreased by 1.
            level--;
        }
    }
}
Employee* DoublySkipList::search(int search_id){
    DoublySkipList_Node* current = head->get_next();
    while(current!=NULL){ // if current equals to null, it means there is no search_id.
        if(current->get_next()->get_data()==NULL){ // end node.
            current = current->get_below();
        }
        else if(current->get_next()->get_data()->get_id()==search_id){
            return current->get_next()->get_data();
        }
        else if(current->get_next()->get_data()->get_id()<search_id){
            current = current->get_next();
        }
        else //next id greater than search_id.
            current = current->get_below();
    }
    return NULL;
}
void DoublySkipList::remove(int remove_id){
    if(search(remove_id) == NULL){ //controling if remove_id is exist.
        cout<< "Invalid id to delete."<<"\n";
        return;
    }
    DoublySkipList_Node* current = head->get_next();
    while(current!=NULL){
        if(current->get_next()->get_data()==NULL){
            current = current->get_below();
        }
        else if(current->get_next()->get_data()->get_id()==remove_id){
            DoublySkipList_Node* deletenode = current->get_next();
            current->set_next(deletenode->get_next());
            if(deletenode->get_below()==nullptr){ //if delete node is last node in column, delete data and node
                delete deletenode->get_data();
                delete deletenode;
                break;  
            }
            else{
                current = current->get_below();
            }
            delete deletenode;
        }
        else if(current->get_next()->get_data()->get_id()<remove_id){
            current = current->get_next();
        }
        else
            current = current->get_below();
    }
}
void DoublySkipList::dumpToFile(ofstream &output_file){
    if (!output_file.is_open()) {
        cout << "Failed to open the output CSV file." << endl;
        return;
    }
    DoublySkipList_Node* current = head->get_next();
    while(current->get_below()!=NULL){ //reach the bottom level
        current = current->get_below();
    }

    output_file << "Employee_ID;Salary;Department\n";
    while(current->get_next()->get_data()!=NULL){ //printing until end node.
        output_file << current->get_next()->get_data()->get_id() << ";";
        output_file << current->get_next()->get_data()->get_salary() << ";";
        output_file << current->get_next()->get_data()->get_department() << "\n";
        current = current->get_next();
    }
    output_file.close();
}
void DoublySkipList::deletesl(){ // delete every nodes and their datas in skip list.
    DoublySkipList_Node* current = head;
    current=current->get_next();
    delete head;
    while (current != nullptr) {
        DoublySkipList_Node* currentNode = current;
        DoublySkipList_Node* nextNode = nullptr;

        while (currentNode != nullptr) {
            nextNode = currentNode->get_next();
            if (currentNode->get_below()==nullptr){
                delete currentNode->get_data();
            }
            delete currentNode;
            currentNode = nextNode;
        }
        current = current->get_below();
    }
}
int find_max_id(int a,int b){ //for finding max id.
    if(a>b)
        return a;
    else 
        return b;

}
int implement_skiplist(DoublySkipList* skiplist, ifstream &inputfile){
    if (!inputfile.is_open()) {
        cerr << "Failed to open the CSV file." << endl;
        return -1;
    }
    int id, salary, department;
    int max_id = 0; 
    string line;
    getline(inputfile,line);

     while (getline(inputfile, line)) {
        istringstream ss(line);
        string field;

        if (getline(ss, field, ';'))
            id = stoi(field);
        if (getline(ss, field, ';'))
            salary = stoi(field);
        if (getline(ss, field, ';'))
            department = stoi(field);
            
        Employee* e = new Employee(id,salary,department);
        max_id = find_max_id(max_id,e->get_id()); //keep max id every time. 
        skiplist->insert(e);
    }
    return max_id; 
    
}
void operation(DoublySkipList* skiplist,ifstream &operationfile,int max_id){ //operations

    if (!operationfile.is_open()) {
        cerr << "Failed to open the CSV file." << endl;
        return;
    }
    string operation;
    string line2;
    while (getline(operationfile, line2)) {
        istringstream ss(line2);
        string field2;
        int newsalary, newdepartment, idToUpdate, idToDelete;

        if(getline(ss, field2, ';'))
            operation = field2;
       
        if (operation == "DELETE" || operation == "DELETE\r") {
            getline(ss, field2, ';');
            idToDelete = stoi(field2);
            skiplist->remove(idToDelete);
        }
        else if (operation == "UPDATE" || operation == "UPDATE\r") {
            getline(ss, field2, ';');
            idToUpdate = stoi(field2);
            getline(ss, field2, ';');
            newsalary = stoi(field2);
            getline(ss, field2, ';');
            newdepartment = stoi(field2);
            if (skiplist->search(idToUpdate)==NULL){ 
                cout<< "Invalid Id to Update."<<"\n";
            }
            else {
                Employee* e = skiplist->search(idToUpdate);
                e->set_salary(newsalary);
                e->set_department(newdepartment);          
            }
        } 
        else if (operation == "ADD" || operation == "ADD\r") {
            getline(ss, field2, ';');
            newsalary = stoi(field2);
            getline(ss, field2, ';');
            newdepartment = stoi(field2);
            max_id++;
            Employee* newemp = new Employee(max_id,newsalary,newdepartment);
            skiplist->insert(newemp);
        }
    }

    operationfile.close();
}
int main(int argc, char** argv){
    srand(time(NULL));
    DoublySkipList* sl = new DoublySkipList(100); // 2 , 10 , 100 , 1000
    
    ifstream inputfile(argv[1]);
    int max_id = implement_skiplist(sl,inputfile);
    
    ifstream operationfile(argv[2]);
    operation(sl,operationfile,max_id);

    ofstream output("doubly_output.csv");
    sl->dumpToFile(output);

    sl->deletesl();
    delete sl;
    return 0;
}