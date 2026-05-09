#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


/**
 * @brief Task will be a class with multiple structs
 *        - User:
 *          - id: int
 *          - name: string
 *          - email: string
 *          - password: string
 *          - created_at: datetime
 *          - updated_at: datetime
 *        - Task:
 *          - id: int
 *          - name: string
 * 
 */


struct Name {
    std::string first_name;
    std::string last_name;
    std::string full_name() {
        return first_name + " " + last_name;
    }
    Name(std::string first_name, std::string last_name) : first_name(first_name), last_name(last_name) {}
    Name() : first_name(""), last_name("") {}
    ~Name() {}
    Name(const Name& other) : first_name(other.first_name), last_name(other.last_name) {}
    Name& operator=(const Name& other) {
        if (this != &other) {
            first_name = other.first_name;
            last_name = other.last_name;
        }
        return *this;
    }
    std::string get_first_name() {return this->first_name;}
    std::string get_last_name() {return this->last_name;}
    void set_first_name(std::string first_name) {this->first_name = first_name;}
    void set_last_name(std::string last_name) {this->last_name = last_name;}
    
};

 struct User {
    int id;
    Name name;
    
 };


class Task {
private:
    

};





int main() {

}