#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>
using namespace std;

// Policy class definition
class Policy {
protected:
    string policyType;
    string expiryDate;
    double premium;
    string additionalDetails;

public:
    Policy(string type, string date, double premium, string details)
        : policyType(type), expiryDate(date), premium(premium), additionalDetails(details) {}

    virtual void displayPolicy() {
        cout << "Policy Type: " << policyType << endl;
        cout << "Expiry Date: " << expiryDate << endl;
        cout << "Premium: $" << fixed << setprecision(2) << premium << endl;
        cout << "Additional Details: " << additionalDetails << endl;
    }

    string getPolicyType() const {
        return policyType;
    }

    string getExpiryDate() const {
        return expiryDate;
    }

    double getPremium() const {
        return premium;
    }
};

// Different Policy Types
class CarInsurance : public Policy {
public:
    CarInsurance(string date, double premium)
        : Policy("Car Insurance", date, premium, "Car coverage details.") {}
};

class HomeInsurance : public Policy {
public:
    HomeInsurance(string date, double premium)
        : Policy("Home Insurance", date, premium, "Home coverage details.") {}
};

class HealthInsurance : public Policy {
public:
    HealthInsurance(string date, double premium)
        : Policy("Health Insurance", date, premium, "Health coverage details.") {}
};

class EducationInsurance : public Policy {
public:
    EducationInsurance(string date, double premium)
        : Policy("Education Insurance", date, premium, "Education coverage details.") {}
};

// Base class for all roles
class Client {
protected:
    string userName;
    int userID;
    vector<Policy*> policies; // The vector to store policies

public:
    Client(string name, int id) : userName(name), userID(id) {}

    virtual void login() {
        cout << "Client Login: " << endl;
    }

    virtual void displayOptions() {
        cout << "1. View current policies." << endl;
        cout << "2. Buy a new policy." << endl;
        cout << "3. File a claim." << endl;
        cout << "4. Exit." << endl;
    }

    virtual void displayPolicies() {
        cout << "User Policies:" << endl;
        if (policies.empty()) {
            cout << "No policies found." << endl;
        } else {
            for (auto& policy : policies) {
                policy->displayPolicy();
                cout << endl;
            }
        }
    }

    // Adding functionality to buy a new policy
    virtual void buyPolicy(map<string, string>& clientAgentMap, map<string, vector<pair<string, string>>>& agentClientsMap, map<string, double>& agentCommission) {
        int choice;
        string agentName;

        cout << "Select a policy to buy:" << endl;
        cout << "1. Car Insurance" << endl;
        cout << "2. Home Insurance" << endl;
        cout << "3. Health Insurance" << endl;
        cout << "4. Education Insurance" << endl;
        cin >> choice;

        cout << "Enter the name of your agent: ";
        cin >> agentName;
        clientAgentMap[userName] = agentName;

        Policy* newPolicy = nullptr;
        switch (choice) {
            case 1:
                newPolicy = new CarInsurance("2025-12-31", 1200);
                break;
            case 2:
                newPolicy = new HomeInsurance("2026-05-30", 800);
                break;
            case 3:
                newPolicy = new HealthInsurance("2026-10-15", 1500);
                break;
            case 4:
                newPolicy = new EducationInsurance("2024-09-01", 2000);
                break;
            default:
                cout << "Invalid choice!" << endl;
                return;
        }

        if (newPolicy) {
            policies.push_back(newPolicy);
            cout << newPolicy->getPolicyType() << " added to your policies!" << endl;

            // Update agent's clients and commission
            agentClientsMap[agentName].push_back(make_pair(userName, newPolicy->getPolicyType()));
            agentCommission[agentName] += 100.00; // $100 commission per policy
        }
    }

    virtual void fileClaim() {
        if (policies.empty()) {
            cout << "You have no policies to file a claim for!" << endl;
            return;
        }
        displayPolicies();
        int choice;
        cout << "Select a policy to file a claim for (1-" << policies.size() << "): ";
        cin >> choice;
        if (choice >= 1 && choice <= policies.size()) {
            cout << "Filing a claim for " << policies[choice - 1]->getPolicyType() << "..." << endl;
        } else {
            cout << "Invalid choice!" << endl;
        }
    }

    virtual void updateDetails() {
        cout << "Updating client details..." << endl;
    }

    virtual ~Client() {
        for (auto policy : policies) {
            delete policy;
        }
    }
};

// Derived class for Agent inheriting Client
class Agent : public Client {
protected:
    double commission;

public:
    Agent(string name, int id) : Client(name, id), commission(0) {}

    void login() override {
        cout << "Agent Login: " << endl;
    }

    // Manage Clients and View Policy Details
    void manageClients(map<string, vector<pair<string, string>>>& agentClientsMap) {
        if (agentClientsMap[userName].empty()) {
            cout << "No clients or policies have been added yet." << endl;
        } else {
            cout << "Clients under Agent " << userName << ":" << endl;
            for (auto& client : agentClientsMap[userName]) {
                cout << "Client Name: " << client.first << " | Policy: " << client.second << endl;
            }
        }
    }

    // Track Agent Performance
    void trackPerformance(map<string, vector<pair<string, string>>>& agentClientsMap, map<string, double>& agentCommission) {
        if (agentClientsMap[userName].empty()) {
            cout << "No policies sold yet." << endl;
        } else {
            cout << "Performance Summary for Agent " << userName << ":" << endl;
            cout << "Policies sold: " << agentClientsMap[userName].size() << endl;
            cout << "Monthly Commission: $" << agentCommission[userName] << endl;
        }
    }

    void displayOptions() override {
        cout << "1. Manage Client Base." << endl;
        cout << "2. Track Performance." << endl;
        cout << "3. View Commission." << endl;
        cout << "4. Exit." << endl;
    }

    virtual ~Agent() {}
};

// Derived class for CompanyExecutive inheriting Agent
class CompanyExecutive : public Agent {
public:
    CompanyExecutive(string name, int id) : Agent(name, id) {}

    void login() override {
        cout << "Company Executive Login: " << endl;
    }

    // Track All Clients
    void trackClients(map<string, vector<pair<string, string>>>& agentClientsMap) {
        bool found = false;
        cout << "All Clients and Policies:" << endl;
        for (auto& agent : agentClientsMap) {
            if (!agent.second.empty()) {
                found = true;
                for (auto& client : agent.second) {
                    cout << "Client Name: " << client.first << " | Policy: " << client.second << " | Agent: " << agent.first << endl;
                }
            }
        }
        if (!found) {
            cout << "No clients or policies available yet." << endl;
        }
    }

    // Track All Agents and Their Commissions
    void trackAgents(map<string, double>& agentCommission) {
        bool found = false;
        for (auto& agent : agentCommission) {
            if (agent.second > 0) {
                found = true;
                cout << setw(15) << "Agent Name" << setw(20) << "         Monthly Commission" << endl;
                cout << "---------------------------------------------" << endl;
                cout << setw(15) << agent.first << setw(20) << "$" << fixed << setprecision(2) << agent.second << endl;
            }
        }
        if (!found) {
            cout << "No commissions have been earned by agents yet." << endl;
        }
    }

    void displayOptions() override {
        cout << "1. Track Clients." << endl;
        cout << "2. Track Agents." << endl;
        cout << "3. Exit." << endl;
    }

    virtual ~CompanyExecutive() {}
};

// Function to handle login
int loginUser(const string users[], const int passwords[], int size) {
    string userName;
    int password;

    cout << "Enter Username: ";
    cin >> userName;
    cout << "Enter Password: ";
    cin >> password;

    for (int i = 0; i < size; ++i) {
        if (users[i] == userName && passwords[i] == password) {
            cout << "Login successful!" << endl;
            return i;
        }
    }

    cout << "Login failed! Invalid credentials." << endl;
    return -1;
}

int main() {
    string clients[5] = {"jk", "jin", "rm", "jhope", "suga"};
    int clientPasswords[5] = {1, 2, 3, 4, 5};

    string agents[5] = {"tannya", "khushi", "keshav", "shree", "krati"};
    int agentPasswords[5] = {1, 2, 3, 4, 5};

    string companyExecs[3] = {"niyati", "muskan", "pranami"};
    int companyExecPasswords[3] = {3, 4, 7};

    // Maps to hold client-agent relationships and agent commissions
    map<string, string> clientAgentMap; // Maps client to their agent
    map<string, vector<pair<string, string>>> agentClientsMap; // Maps agent to clients and their policies
    map<string, double> agentCommission; // Maps agent to their total commission

    while (true) { // Keep prompting for login until exit
        int userType;
        cout << "Select your portal: " << endl;
        cout << "1. Client" << endl;
        cout << "2. Agent" << endl;
        cout << "3. Company Executive" << endl;
        cout << "4. Exit" << endl;
        cin >> userType;

        if (userType == 1) { // Client
            int clientIndex = loginUser(clients, clientPasswords, 5);
            if (clientIndex != -1) {
                Client client(clients[clientIndex], clientPasswords[clientIndex]);
                client.login();

                int choice;
                do {
                    client.displayOptions();
                    cin >> choice;
                    switch (choice) {
                        case 1:
                            client.displayPolicies();
                            break;
                        case 2:
                            client.buyPolicy(clientAgentMap, agentClientsMap, agentCommission);
                            break;
                        case 3:
                            client.fileClaim();
                            break;
                    }
                } while (choice != 4);
            }

        } else if (userType == 2) { // Agent
            int agentIndex = loginUser(agents, agentPasswords, 5);
            if (agentIndex != -1) {
                Agent agent(agents[agentIndex], agentPasswords[agentIndex]);
                agent.login();

                int choice;
                do {
                    agent.displayOptions();
                    cin >> choice;
                    switch (choice) {
                        case 1:
                            agent.manageClients(agentClientsMap);
                            break;
                        case 2:
                            agent.trackPerformance(agentClientsMap, agentCommission);
                            break;
                        case 3:
                            cout << "Total Monthly Commission: $" << agentCommission[agents[agentIndex]] << endl;
                            break;
                    }
                } while (choice != 4);
            }

        } else if (userType == 3) { // Company Executive
            int execIndex = loginUser(companyExecs, companyExecPasswords, 3);
            if (execIndex != -1) {
                CompanyExecutive exec(companyExecs[execIndex], companyExecPasswords[execIndex]);
                exec.login();

                int choice;
                do {
                    exec.displayOptions();
                    cin >> choice;
                    switch (choice) {
                        case 1:
                            exec.trackClients(agentClientsMap);
                            break;
                        case 2:
                            exec.trackAgents(agentCommission);
                            break;
                    }
                } while (choice != 3);
            }
        } else if (userType == 4) {
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Invalid choice, please try again!" << endl;
        }
    }

    return 0;
}
