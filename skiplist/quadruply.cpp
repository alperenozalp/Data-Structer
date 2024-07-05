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

class QuadruplySkipList_Node{
    private:
        Employee* data;
        QuadruplySkipList_Node* next;
        QuadruplySkipList_Node* prev;
        QuadruplySkipList_Node* below;
        QuadruplySkipList_Node* above;
    public:
        QuadruplySkipList_Node(Employee *data){
            this->data = data;
            this->next = nullptr;
            this->prev = nullptr;
            this->below = nullptr;
            this->above = nullptr;
        };
        ~QuadruplySkipList_Node(){};
        //getters and setters
        void set_data(Employee* data){this->data = data;}
        void set_next(QuadruplySkipList_Node* next){this->next = next;}
        void set_prev(QuadruplySkipList_Node* prev){this->prev = prev;}
        void set_below(QuadruplySkipList_Node* below){this->below = below;}
        void set_above(QuadruplySkipList_Node* above){ this->above = above;}
        Employee* get_data(){return this->data;}
        QuadruplySkipList_Node* get_next(){return this->next;}
        QuadruplySkipList_Node* get_prev(){return this->prev;}
        QuadruplySkipList_Node* get_below(){return this->below;}
        QuadruplySkipList_Node* get_above(){ return this->above;}
};

class QuadruplySkipList{
    private:
        QuadruplySkipList_Node* head;
        int height;
    public:
        QuadruplySkipList(int height_in);
        ~QuadruplySkipList();
        void insert(Employee* data);
        void remove(int remove_id);
        Employee* search(int search_id);
        void dumpToFile(ofstream& out_file);
        void update(int id,int salary, int department);
        int max_id();
        void deletesl();

        
};
QuadruplySkipList::QuadruplySkipList(int height_in){

    this->height = height_in;
    
    /*
    create head node and connect it to starting nodes
    add starting and end nodes up to height
    connect starting nodes to end nodes
    */
    head = new QuadruplySkipList_Node(nullptr);
    QuadruplySkipList_Node* start = new QuadruplySkipList_Node(nullptr);
    QuadruplySkipList_Node* end = new QuadruplySkipList_Node(nullptr);
    head->set_next(start);
    start->set_prev(head);
    start->set_next(end);
    end->set_prev(start);
    QuadruplySkipList_Node* currstart = start;
    QuadruplySkipList_Node* currend = end;
    for(int i=1;i<height;i++){
        QuadruplySkipList_Node* start = new QuadruplySkipList_Node(nullptr);
        QuadruplySkipList_Node* end = new QuadruplySkipList_Node(nullptr);
        currstart->set_below(start);
        start->set_above(currstart);
        currend->set_below(end);
        end->set_above(currend);
        currstart = start;
        currend = end;
        currend->set_prev(currstart);
        currstart->set_next(currend);
    }
    
}

QuadruplySkipList::~QuadruplySkipList() {
    
}

void QuadruplySkipList::insert(Employee* data){
    // How many time this employee will be inserted?
    int number_of_add = 1;  
    while (number_of_add < height && rand()%2==1){
        number_of_add++;
    }     
    QuadruplySkipList_Node* current = head->get_next();
    QuadruplySkipList_Node* newnode = new QuadruplySkipList_Node(data);
    while(true){
        if (current->get_next()->get_data()==NULL){
            if(current->get_below()==NULL){ //next node is end node and below is NULL.
                //insert
                QuadruplySkipList_Node* tempnext = current->get_next();
                for(int i = 1;i<=number_of_add;i++){
                    current->set_next(newnode);
                    newnode->set_prev(current);
                    newnode->set_next(tempnext);
                    tempnext->set_prev(newnode);
                    if(i!=number_of_add){//This part will not work for last insertion operation.
                        QuadruplySkipList_Node* extranode = new QuadruplySkipList_Node(data);
                        newnode->set_above(extranode);
                        extranode->set_below(newnode);
                        newnode = extranode;
                        while(true){
                            if(current->get_above()!=NULL){
                                current=current->get_above();
                                break;    
                            }
                            else{
                                current = current->get_prev();
                            }
                        }
                        while(true){
                            if(tempnext->get_above()!=NULL){
                                tempnext = tempnext->get_above();
                                break;
                            }
                            else{
                                tempnext = tempnext->get_next();
                            }
                        }
                    }
                }
                break;
            }
            else
                current = current->get_below();
        }
        else if (current->get_next()->get_data()->get_id() > data->get_id()){
            if(current->get_below()==NULL){ //next node's id is larger than data's id and below is NULL.
                //insert
                QuadruplySkipList_Node* tempnext = current->get_next();
                for(int i = 1;i<=number_of_add;i++){
                    current->set_next(newnode);
                    newnode->set_prev(current);
                    newnode->set_next(tempnext);
                    tempnext->set_prev(newnode);
                    if(i!=number_of_add){//This part will not work for last insertion operation.
                        QuadruplySkipList_Node* extranode = new QuadruplySkipList_Node(data);
                        newnode->set_above(extranode);
                        extranode->set_below(newnode);
                        newnode = extranode;
                        while(true){
                            if(current->get_above()!=NULL){
                                current=current->get_above();
                                break;    
                            }
                            else{
                                current = current->get_prev();
                            }
                        }
                        while(true){
                            if(tempnext->get_above()!=NULL){
                                tempnext = tempnext->get_above();
                                break;
                            }
                            else{
                                tempnext = tempnext->get_next();
                            }
                        }
                    }
                }
                break;
            }
            else
                current = current->get_below();
        }
        else if (current->get_next()->get_data()->get_id() < data->get_id()){
            current = current->get_next();
        }
    }
}    

Employee* QuadruplySkipList::search(int search_id){
    QuadruplySkipList_Node* current = head->get_next(); 
    while(current->get_next()!=nullptr){
        if(current->get_next()->get_data()==NULL){
            if (current->get_below()!=NULL){
                current = current->get_below();
            }
            else{
                break;
            }
        }
        else if (current->get_next()->get_data()->get_id() == search_id){
            return current->get_next()->get_data();
            break;
        }
        else if (current->get_next()->get_data()->get_id() < search_id){
            current = current->get_next();
        }
        else if (current->get_next()->get_data()->get_id() > search_id){
            if (current->get_below()!=NULL){
                current = current->get_below();
            }
            else{
                break;
            }
        }

    }
    return NULL;
}

void QuadruplySkipList::remove(int remove_id) {
   QuadruplySkipList_Node* current = head->get_next(); 
    while(true){
        if(current->get_next()->get_data()==NULL){
            if (current->get_below()!=NULL){
                current = current->get_below();
            }
            else{
                cout << "Invalid ID to Delete.";
                break;
            }
        }
        else if (current->get_next()->get_data()->get_id() == remove_id){
            current=current->get_next();
            while (current->get_below()!=nullptr)
            {
                current->get_next()->set_prev(current->get_prev());
                current->get_prev()->set_next(current->get_next());
                current->set_above(nullptr);
                current=current->get_below();
                delete current->get_above();
            }
            current->get_next()->set_prev(current->get_prev());
            current->get_prev()->set_next(current->get_next());
            current->set_above(nullptr);
            delete current->get_data();
            delete current;
            break;
        }
        else if (current->get_next()->get_data()->get_id() < remove_id){
            current = current->get_next();
        }
        else if (current->get_next()->get_data()->get_id() > remove_id){
            if (current->get_below()!=NULL){
                current = current->get_below();
            }
            else{
                cout << "Invalid ID to Delete.";
                break;
            }
        }

    }   
}


void QuadruplySkipList::update(int id, int salary,int department){
    QuadruplySkipList_Node* current = head->get_next(); 
    while(true){
        if(current->get_next()->get_data()==NULL){
            if (current->get_below()!=NULL){
                current = current->get_below();
            }
            else{
                cout << "Invalid ID to Update.";
                break;
            }
        }
        else if (current->get_next()->get_data()->get_id() == id){
            QuadruplySkipList_Node* updatenode = current->get_next();
            updatenode->get_data()->set_salary(salary);
            updatenode->get_data()->set_department(department);
            break;
        }
        else if (current->get_next()->get_data()->get_id() < id){
            current = current->get_next();
        }
        else if (current->get_next()->get_data()->get_id() > id){
            if (current->get_below()!=NULL){
                current = current->get_below();
            }
            else{
                cout << "Invalid ID to Update.";
                break;
            }
        }

    }
    
}

void QuadruplySkipList::dumpToFile(ofstream &output_file){
    if (!output_file.is_open()) {
        cout << "Failed to open the output CSV file." << endl;
        return;
    }
    QuadruplySkipList_Node* current = head->get_next();
    while(current->get_below()!=NULL){
        current = current->get_below();
    }

    output_file << "Employee_ID;Salary;Department\n";
    while(current->get_next()->get_data()!=NULL){
        output_file << current->get_next()->get_data()->get_id() << ";";
        output_file << current->get_next()->get_data()->get_salary() << ";";
        output_file << current->get_next()->get_data()->get_department() << "\n";
        current = current->get_next();
    }
    
}

int QuadruplySkipList::max_id(){
    QuadruplySkipList_Node* current = head->get_next();
    while(true){ // find end nodes 
        if(current->get_next()->get_data()==NULL){
            current = current->get_next(); // end node
            while(current->get_below()!=NULL){
                current = current->get_below();
            }
            return current->get_prev()->get_data()->get_id();
        }
        else
            current = current->get_next();
    }
    
}

void implement_skiplist(QuadruplySkipList* skiplist, ifstream &inputfile){
    if (!inputfile.is_open()) {
        cerr << "Failed to open the CSV file." << endl;
        return;
    }
    int id, salary, department;
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

        skiplist->insert(e);
        
    }
    inputfile.close();
}

void operation(QuadruplySkipList* skiplist,ifstream &operationfile){

    if (!operationfile.is_open()) {
        cerr << "Failed to open the CSV file." << endl;
        return;
    }
    string operation;
    string line2;
    int max_id = skiplist->max_id();

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
            skiplist->update(idToUpdate,newsalary,newdepartment);            
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
void QuadruplySkipList::deletesl(){
    QuadruplySkipList_Node* current= head;
    current=current->get_next();
    delete current->get_prev();
    while (current != nullptr) {
        QuadruplySkipList_Node* currentNode = current;
        QuadruplySkipList_Node* nextNode = nullptr;

        while (currentNode != nullptr) {
            nextNode = currentNode->get_next();
            if (currentNode->get_below()==nullptr)
            {
                delete currentNode->get_data();
            }
            
            delete currentNode;
            currentNode = nextNode;
        }

        current= current->get_below();
    }
}
int main(int argc, char** argv){
    srand(time(NULL));
    QuadruplySkipList* sl = new QuadruplySkipList(100); // 2, 10, 100, 1000

    ifstream inputfile(argv[1]);
    implement_skiplist(sl,inputfile);

    ifstream operationfile(argv[2]);
    operation(sl,operationfile);

    ofstream output("quadruply_output.csv");
    sl->dumpToFile(output);
    output.close();

    sl->deletesl();
    delete sl;
    return 0;

}