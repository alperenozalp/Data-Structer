#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iterator>
#include "doublelinklist.h"
#include "objects.h"

using namespace std;
template<typename T>
struct Single {
        T data;
        Single<T>* next = NULL;
};

template<typename T>
struct List
{
        Single<T>* head = NULL;
        void add_front(T);
        void remove_front();
};
template<class T>
void List<T>::add_front(T e) {
    Single<T>* newnode = new Single<T>;
    newnode->data=e;
    if(head==NULL){
        head=newnode;
        return;
    }
    newnode->next=head;
    head = newnode;
}
template<class T>
void List<T>::remove_front(){
    if(head==NULL){
        return;
    }
    Single<T>* temp = head;
    head = head->next;
    delete temp;
}

template <typename T>
struct Stack{
    private:
        List<T> data;
    public:
        int elem_count = 0;
        void push(T e);
        void pop();
        T top();
        void delete_all();
};
template <typename T>
void Stack<T>::delete_all(){
    while(this->data.head!=NULL){
        this->data.remove_front();
    }
}
template <typename T>
void Stack<T>::push(T e){
    this->data.add_front(e);
    elem_count++;
}
template <typename T>
void Stack<T>::pop(){
    if(this->data.head!=NULL){
        this->data.remove_front();
        elem_count--;
    }
}
template <typename T>
T Stack<T>::top(){
    return this->data.head->data;
}

void solve_game(Stack<GameState> game_states){ //stack that keeping game states
    GameState state = game_states.top();    
    game_states.pop();                      // popped current state from stack to avoid try again 
    if(state.win){              
        cout<<"*** You Reached The FREEDOM !!! ***\n";
        game_states.delete_all();
        return;
    }
    while(!state.win){
        int current_room = state.room_id;
        int object_count = state.rooms.get(current_room)->room_objects.elemcount;
        for(int j=0;j<object_count;j++){ //actions
            for(int i=1;i<=5;i++){ //objects in room
                GameState next = state;                         //This is to go to the previous state in case of a state where the room is changed.
                int check = next.advance(i,j);
                if(check>0 && next.room_id==current_room){                   
                    game_states.push(next);                 // The positive ID received. So with the last state start function again.
                    solve_game(game_states);
                    return;                                 // Prevents the function from continuing when the recursive finished.
                }                 
                else if(next.room_id!=current_room){                // If action cause the changing room we push state to stack and we continue to             
                    game_states.pop();                              // Before push the other room state , Poping the last state because if we reach more current 
                    game_states.push(next);                         // state, the previous state does not matter.    
                                                                    // Doing this to avoid changing room before making the correct choise.
                }                                                   // try remaining action-object pairs until before go to other room.                                                                               
            }
        }
        // There is no further progress in the current room, so it is continued from the last pushed room.
        state = game_states.top();    
        game_states.pop();
        
    }
}

void solve_game_2(GameState state){
    Stack<GameState> states;    
    states.push(state);
    while (state.win!=1){
        GameState current = states.top();   
        states.pop();
        int current_room = current.room_id;
        int object_count = current.rooms.get(current_room)->room_objects.elemcount;
        for(int j=0;j<object_count;j++){  
            for(int i=1;i<=5;i++){ 
                GameState temp = current;
                int check = temp.advance(i,j);
                if(check>0 && temp.room_id==current.room_id){
                    solve_game_2(temp);
                    states.delete_all();    // Prevent the MemoryLeak
                    return;                 // Prevents the function from continuing when the recursive finished.
                }
                if(temp.room_id!=current.room_id){
                    states.pop();           // Before push the other room state , Poping the last state because if we reach more current         
                    states.push(temp);      // state, the previous state does not matter.   o            
                }                           // If action cause the changing room we push state to stack and we continue to             
            }  
        }
    }
    cout<<"*** You Reached The FREEDOM !!! ***\n";
    states.delete_all();
}
void solve_game_3(GameState state){

    int current_room = state.room_id;
    int object_count = state.rooms.get(current_room)->room_objects.elemcount;
    
        for(int i=1;i<=5;i++){  
            for(int j=0;j<object_count;j++){ 


            }

        }


}


int main()
{
    GameState first_state;
    first_state.create_init_state();
    //Stack<GameState> game_state;
    //game_state.push(first_state); 
    //solve_game(game_state); 
    
    //solve_game_2(first_state);
    solve_game_2(first_state);
    /*while (!first_state.lose && !first_state.win)
    { 
        first_state.print_situation();
        int action, object;
        cout << "Select action: 1-Open 2-Look At 3-Pick Up 4- Misbehave 5- Talk To: ";
        cin >> action;
        cout << "Select object: ";
        cin >> object;
        first_state.advance(action, object);//You may use effectID to check whether your last action changed something...
    }*/


    return 0;
}

