#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

class Node
{
public:
    string name;
    int hour;
    int minute;
    int executionTime;
    int waitingTime;
    int executedHour; // hours at time of execution
    int executedMinute; // minutes at time of execution
    Node* next;
    Node() {} // default ctor

    Node(string Name, int Hour, int Minute, int executeTime, int waiting = 0) :
    name(Name), hour(Hour), minute(Minute), executionTime(executeTime), waitingTime(waiting), executedHour(Hour), executedMinute(Minute), next(nullptr) {}

    ~Node(){}
};

class Queue
{
private:
    int numberOfJobs=0;
    int totalJobTime=0;
    int totalWaitingTime=0;
public:
    Node *head, *tail;
    Queue() : head(nullptr), tail(nullptr) {}
    ~Queue()
    {
        while (head)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // pushing the new inserted node in its position based on its priority
    void push(string deviceName, int hourReceived, int minuteReceived, int exTime)
    {
        numberOfJobs++;
        totalJobTime += exTime;
        Node *newNode = new Node(deviceName, hourReceived, minuteReceived, exTime);
        if (head == nullptr) // Queue is empty
        {
            head = newNode;
            tail = newNode;
        }

        else // Putting node in its right order
        {
            Node *current = head;
            Node *previous = nullptr;
            Node *putInPlace = newNode;

            while (current)
            {
                // the new node has the highest priority (smallest hour)
                if (putInPlace->hour < current->hour)
                {
                    if (previous == nullptr) // new node is the head
                    {
                        putInPlace->next = head;
                        head = putInPlace;
                        return;
                    }
                    else
                    {
                        previous->next = putInPlace;
                        putInPlace->next = current;
                        return;
                    }
                }
                // same hour -> compare minutes
                else if (putInPlace->hour == current->hour)
                {
                    // smaller minutes
                    if (putInPlace->minute < current->minute)
                    {
                        if (previous == nullptr) // check if head
                        {
                            putInPlace->next = head;
                            head = putInPlace;
                            return;
                        }
                        else
                        {
                            previous->next = putInPlace;
                            putInPlace->next = current;
                            return;
                        }
                    }
                    // same minute -> compare execution time
                    else if (putInPlace->minute == current->minute)
                    {
                        if (putInPlace->executionTime < current->executionTime)
                        {
                            if (previous == nullptr) // check if head
                            {
                                putInPlace->next = head;
                                head = putInPlace;
                                return;
                            }
                            else
                            {
                                previous->next = putInPlace;
                                putInPlace->next = current;
                                return;
                            }
                        }
                    }
                }
                // move iterators / pointers
                previous = current;
                current = current->next;
            }
            // End of Queue / least priority / highest received time
            if (previous != nullptr)
            {
                previous->next = putInPlace;
                tail = putInPlace;
                putInPlace->next = nullptr;
            }
        }
    }

    // calculate the waiting time and adjusting the overlapped job's starting time
    void calcWaitingTime()
    {
        Node *current = head;
        if (current == nullptr)
            return;

        int currentHour=current->hour;
        int currentMinute=current->minute;

        // head execution time, as it in not in the while loop. (it is not affected because it has no nodes before it)
        current->executedHour = currentHour;
        current->executedMinute = currentMinute;

        // finishHour, finishMinute will be saving the time: when the printer is available
        int finishHour = currentHour;
        int finishMinute = currentMinute + current->executionTime;
        if (finishMinute >= 60) // handling edge case where the minutes get out of range (+60 minutes)
        {
            finishHour += finishMinute / 60;
            finishMinute = finishMinute % 60;
        }

        current = current->next;
        while (current)
        {
            int scheduledTotalTime = current->hour * 60 + current->minute; // in minutes for easier calculations
            int lastFinishTotalTime = finishHour * 60 + finishMinute; // in minutes for easier calculations

            // overlapped jobs -> add waiting time & shift the starting time
            if (lastFinishTotalTime > scheduledTotalTime)
            {
                current->waitingTime = lastFinishTotalTime - scheduledTotalTime; // adjusting the waiting time
                totalWaitingTime += current->waitingTime; // -> for the getAverageWaitingTime function
                int actualStart = lastFinishTotalTime;
                current->executedHour = actualStart / 60; // adjusting the new execution hour of node
                current->executedMinute = actualStart % 60; // adjusting the new execution minutes of node
            }
            else // NO OVERLAPPING IN JOB TIMES AND EXECUTIONS
            {
                current->waitingTime = 0;
                current->executedHour = current->hour;
                current->executedMinute = current->minute;
            }

            // updating the time the printer is available at for a job
            int availableForJob = current->executedHour * 60 + current->executedMinute + current->executionTime; // in minutes for easier calculations
            finishHour = availableForJob / 60;
            finishMinute = availableForJob % 60;

            current = current->next;
        }
    }

    int getNumberOfJobs() { return numberOfJobs; }
    int getTotalTimeOccupied() { return totalJobTime; }
    float getAverageWaitingTime()
    {
        if (totalWaitingTime!=0) // to avoid the return value: "nan"
            return (float)totalWaitingTime / numberOfJobs;
        else
            return 0;
    }

    void getSummary()
    {
        calcWaitingTime();
        cout<<"Jobs Executed: "<<getNumberOfJobs()<<endl;
        Node *current = head;
        while (current!=0)
        {
            cout<<current->name<<"  "<<setw(2)<<setfill('0')<<current->hour<<":"<<setw(2)<<setfill('0')<<current->minute<<" "
            <<current->executionTime<<"    "<<"executed at "<<setw(2)<<setfill('0')<<current->executedHour
            <<":"<<setw(2)<<setfill('0')<<current->executedMinute<<"   "<<"waiting time: "<<current->waitingTime<<endl;
            current = current->next;
        }

        cout<<"The printer was occupied for "<<getTotalTimeOccupied()<<" Minutes"<<endl;
        cout<<"Average waiting time for all jobs is "<<getAverageWaitingTime()<<" minutes"<<endl;
    }

};


int main()
{
    Queue queue;
    ifstream infile("Printer.txt");
    if (!infile)
    {
        cout << "Can't open file!" << endl;
        return 0;
    }

    int numberOfJobs;
    infile >> numberOfJobs;
    string name;
    int hour, minute, exTime;
    for (int i = 0; i < numberOfJobs; ++i)
    {
        infile >> name;
        infile >> hour;
        infile.ignore(); // ignore the ':'
        infile >> minute;
        infile >> exTime;
        queue.push(name, hour, minute, exTime);
    }
    infile.close();
    queue.getSummary();

}