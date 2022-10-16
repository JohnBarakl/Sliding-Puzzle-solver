//
// Created by Ioannis Baraklilis on 26/3/21.
// Comments in UTF-8 encoding.
//

#include <iostream>
#include <stack>
#include <unordered_set>
#include <queue>
#include <chrono>

#include "Puzzle.h"

/*
 * Ψάχνει λύση στο πρόβλημα με πλακίδια απο την αρχική κατάσταση initial στην τελική goal χρησιμοποιώντας τον αλγόριθμο τυφλής αναζήτησης
 * Αναζήτηση Πρώτα σε Βάθος / Depth-First Search / DFS.
 *
 * Αν η λύση βρεθεί, επιστρέφεται πίνακας με δείκτες των καταστάσεων του μονοπατιού (ακολουθία καταστάσεων) απο την αρχική μέχρι την τελική.
 * Αν η λύση δεν βρεθεί επιστρέφεται κενός πίνακας.
 *
 * Η παράμετρος statesExpanded θέτει ως τιμή τον αριθμό των καταστάσεων που εξερευνήθηκαν.
 * Η παράμετρος maxTotalStatesInMemory θέτει ως τιμή τον μέγιστο συνολικό αριθμό των καταστάσεων που υπήρξαν σε κάποιο σημείο εκτέλεσης του αλγορίθμου.
 */
std::vector<Puzzle *> DFS(Puzzle *initial, Puzzle *goal, unsigned long &statesExpanded, unsigned long &maxTotalStatesInMemory){
    std::stack<Puzzle *> frontier; // Ορίζω την δομή που αντιστοιχεί στο μέτωπο αναζήτησης. Στοίβα για τον DFS.
    std::unordered_set<Puzzle*, PuzzleHashFunction, PuzzlePtrEquals> closedSet; // Ορίζω την δομή που αντιστοιχεί στο κλειστό σύνολο.
                                                                                // Ως συνάρτηση κατακερματισμού ορίζεται η PuzzleHashFunction και συνάρτηση ισότητας η PuzzlePtrEquals.

    // Ορίζω αρχικές τιμές
    statesExpanded = 0;
    maxTotalStatesInMemory = 0;

    frontier.push(initial); // Εισάγω στο μέτωπο την αρχική κατάσταση.

    Puzzle * currentElement; // Ορίζω δείκτη για το τρέχον στοιχείο που εξετάζεται σε εκάστοτε επανάληψη.

    while (!frontier.empty()){ // Όταν αδειάσει το μέτωπο αναζήτησης, η αναζήτηση λύσης έχει τελειώσει με αποτυχία.
        // Ενημερώνω την μεταβλητή μνήμης.
        if (frontier.size()+closedSet.size() > maxTotalStatesInMemory){
            maxTotalStatesInMemory = frontier.size() + closedSet.size();
        }

        currentElement = frontier.top(); // Παίρνω το πρώτο στοιχείο απο το μέτωπο αναζήτησης.
        frontier.pop(); // Αφαιρώ το στοιχείο που μόλις πήρα.

        if (closedSet.count(currentElement) != 0){ // Αν η κατάσταση υπάρχει στο κλείστο σύνολο (την έχω ξαναεπισκευτεί), την αγνοώ.
            delete currentElement;
            continue;
        }

        // Ενημερώνω τον αριθμό των καταστάσεων που χρειάστηκε να εξερευνήσω.
        statesExpanded++;

        // Βρέθηκε κατάσταση που ταυτίζεται με την τελική. Η αναζήτηση τερμάτισε με επιτυχία. Επιστρέφω πίνακα με το μονοπάτι προς την λύση.
        if (*currentElement == *goal){
            // Αποδεσμεύω την μνήμη που δεσμεύεται και δεν χρειάζεται πια.
            while (!frontier.empty()){
                delete frontier.top();
                frontier.pop();
            }

            std::vector<Puzzle*> path = currentElement->pathToRoot();

            for (Puzzle *p : path){ // Αφαιρώ απο το κλειστό σύνολο τις καταστάσεις του μονοπατιού που δεν θέλω να διαγραφούν.
                if (closedSet.count(p)!=0){
                    closedSet.erase(p);
                }
            }

            for (Puzzle *p : closedSet){ // Διαγράφω τις καταστάσεις του κλειστού συνόλου που δεν οδήγησαν στην λύση
                delete p;
            }

            return path;
        }

        // Επεκτείνω (εφαρμόζω τους τελεστές μετάβασης για) την τρέχουσα κατάσταση και προσθέτω τα παιδιά της στην αρχή του μετώπου αναζήτησης.
        for (Puzzle *ptr : currentElement->expandAll()){
            frontier.push(ptr);
        }

        // Βάζω την κατάσταση γονέα στο κλειστό σύνολο.
        closedSet.insert(currentElement);
    }

    // Αποδεσμεύω την μνήμη που δεσμεύεται και δεν χρειάζεται πια.
    while (!frontier.empty()){
        delete frontier.top();
        frontier.pop();
    }
    for (Puzzle *p : closedSet){ // Διαγράφω τις καταστάσεις του κλειστού συνόλου που δεν οδήγησαν στην λύση
        delete p;
    }
    return std::vector<Puzzle *>(); // Τερματισμός με αποτυχία, επιστρέφω κενό πίνακα.
}

/*
 * Ψάχνει λύση στο πρόβλημα με πλακίδια απο την αρχική κατάσταση initial στην τελική goal χρησιμοποιώντας τον αλγόριθμο τυφλής αναζήτησης
 * Αναζήτηση Πρώτα σε Πλάτος / Breadth-First Search / BFS.
 *
 * Αν η λύση βρεθεί, επιστρέφεται πίνακας με δείκτες των καταστάσεων του μονοπατιού (ακολουθία καταστάσεων) απο την αρχική μέχρι την τελική.
 * Αν η λύση δεν βρεθεί επιστρέφεται κενός πίνακας.
 *
 * Η παράμετρος statesExpanded θέτει ως τιμή τον αριθμό των καταστάσεων που εξερευνήθηκαν.
 * Η παράμετρος maxTotalStatesInMemory θέτει ως τιμή τον μέγιστο συνολικό αριθμό των καταστάσεων που υπήρξαν σε κάποιο σημείο εκτέλεσης του αλγορίθμου.
 */
std::vector<Puzzle *> BFS(Puzzle *initial, Puzzle *goal, unsigned long &statesExpanded, unsigned long &maxTotalStatesInMemory){
    std::queue<Puzzle *> frontier; // Ορίζω την δομή που αντιστοιχεί στο μέτωπο αναζήτησης. Ουρά για τον BFS.
    std::unordered_set<Puzzle*, PuzzleHashFunction, PuzzlePtrEquals> closedSet; // Ορίζω την δομή που αντιστοιχεί στο κλειστό σύνολο.
                                                                                // Ως συνάρτηση κατακερματισμού ορίζεται η PuzzleHashFunction και συνάρτηση ισότητας η PuzzlePtrEquals.
    // Ορίζω αρχικές τιμές
    statesExpanded = 0;
    maxTotalStatesInMemory = 0;

    frontier.push(initial); // Εισάγω στο μέτωπο την αρχική κατάσταση.

    Puzzle * currentElement; // Ορίζω δείκτη για το τρέχον στοιχείο που εξετάζεται σε εκάστοτε επανάληψη.

    while (!frontier.empty()){ // Όταν αδειάσει το μέτωπο αναζήτησης, η αναζήτηση λύσης έχει τελειώσει με αποτυχία.
        // Ενημερώνω την μεταβλητή μνήμης.
        if (frontier.size()+closedSet.size() > maxTotalStatesInMemory){
            maxTotalStatesInMemory = frontier.size() + closedSet.size();
        }

        currentElement = frontier.front(); // Παίρνω το πρώτο στοιχείο απο το μέτωπο αναζήτησης.
        frontier.pop(); // Αφαιρώ το στοιχείο που μόλις πήρα.

        if (closedSet.count(currentElement) != 0){ // Αν η κατάσταση υπάρχει στο κλείστο σύνολο (την έχω ξαναεπισκευτεί), την αγνοώ.
            delete currentElement;
            continue;
        }

        // Ενημερώνω τον αριθμό των καταστάσεων που χρειάστηκε να εξερευνήσω.
        statesExpanded++;

        // Βρέθηκε κατάσταση που ταυτίζεται με την τελική. Η αναζήτηση τερμάτισε με επιτυχία. Επιστρέφω πίνακα με το μονοπάτι προς την λύση.
        if (*currentElement == *goal){
            // Αποδεσμεύω την μνήμη που δεσμεύεται και δεν χρειάζεται πια.
            while (!frontier.empty()){
                delete frontier.front();
                frontier.pop();
            }

            std::vector<Puzzle*> path = currentElement->pathToRoot();

            for (Puzzle *p : path){ // Αφαιρώ απο το κλειστό σύνολο τις καταστάσεις του μονοπατιού που δεν θέλω να διαγραφούν.
                if (closedSet.count(p)!=0){
                    closedSet.erase(p);
                }
            }

            for (Puzzle *p : closedSet){ // Διαγράφω τις καταστάσεις του κλειστού συνόλου που δεν οδήγησαν στην λύση
                delete p;
            }

            return path;
        }

        // Επεκτείνω (εφαρμόζω τους τελεστές μετάβασης για) την τρέχουσα κατάσταση και προσθέτω τα παιδιά της στην αρχή του μετώπου αναζήτησης.
        for (Puzzle *ptr : currentElement->expandAll()){
            frontier.push(ptr);
        }

        // Βάζω την κατάσταση γονέα στο κλειστό σύνολο.
        closedSet.insert(currentElement);
    }

    // Αποδεσμεύω την μνήμη που δεσμεύεται και δεν χρειάζεται πια.
    while (!frontier.empty()){
        delete frontier.front();
        frontier.pop();
    }
    for (Puzzle *p : closedSet){ // Διαγράφω τις καταστάσεις του κλειστού συνόλου που δεν οδήγησαν στην λύση
        delete p;
    }
    return std::vector<Puzzle *>(); // Τερματισμός με αποτυχία, επιστρέφω κενό πίνακα.
}

/*
 * Ψάχνει λύση στο πρόβλημα με πλακίδια απο την αρχική κατάσταση initial στην τελική goal χρησιμοποιώντας τον αλγόριθμο ευρετικής αναζήτησης
 * Αναζήτηση Πρώτα στο Καλύτερο / Best-First Search / BestFS.
 *
 * Αν η λύση βρεθεί, επιστρέφεται πίνακας με δείκτες των καταστάσεων του μονοπατιού (ακολουθία καταστάσεων) απο την αρχική μέχρι την τελική.
 * Αν η λύση δεν βρεθεί επιστρέφεται κενός πίνακας.
 *
 * Η παράμετρος statesExpanded θέτει ως τιμή τον αριθμό των καταστάσεων που εξερευνήθηκαν.
 * Η παράμετρος maxTotalStatesInMemory θέτει ως τιμή τον μέγιστο συνολικό αριθμό των καταστάσεων που υπήρξαν σε κάποιο σημείο εκτέλεσης του αλγορίθμου.
 */
std::vector<Puzzle *> BestFS(Puzzle *initial, Puzzle *goal, unsigned long &statesExpanded, unsigned long &maxTotalStatesInMemory){
    std::priority_queue<Puzzle *, std::deque<Puzzle *>, PuzzleGreaterHeuristicValueComparator> frontier; // Ορίζω την δομή που αντιστοιχεί στο μέτωπο αναζήτησης. Ουρά προτεραιότητας για τον BestFS.
                                                                                                         // Ως συνάρτηση σύκρισης ορίζεται η PuzzleGreaterHeuristicValueComparator.
    std::unordered_set<Puzzle*, PuzzleHashFunction, PuzzlePtrEquals> closedSet; // Ορίζω την δομή που αντιστοιχεί στο κλειστό σύνολο.
                                                                                // Ως συνάρτηση κατακερματισμού ορίζεται η PuzzleHashFunction και συνάρτηση ισότητας η PuzzlePtrEquals.

    // Ορίζω αρχικές τιμές
    statesExpanded = 0;
    maxTotalStatesInMemory = 0;

    frontier.push(initial); // Εισάγω στο μέτωπο την αρχική κατάσταση.

    Puzzle * currentElement; // Ορίζω δείκτη για το τρέχον στοιχείο που εξετάζεται σε εκάστοτε επανάληψη.

    while (!frontier.empty()){ // Όταν αδειάσει το μέτωπο αναζήτησης, η αναζήτηση λύσης έχει τελειώσει με αποτυχία.
        // Ενημερώνω την μεταβλητή μνήμης.
        if (frontier.size()+closedSet.size() > maxTotalStatesInMemory){
            maxTotalStatesInMemory = frontier.size() + closedSet.size();
        }

        currentElement = frontier.top(); // Παίρνω το πρώτο στοιχείο απο το μέτωπο αναζήτησης.
        frontier.pop(); // Αφαιρώ το στοιχείο που μόλις πήρα.

        if (closedSet.count(currentElement) != 0){ // Αν η κατάσταση υπάρχει στο κλείστο σύνολο (την έχω ξαναεπισκευτεί), την αγνοώ.
            delete currentElement;
            continue;
        }

        // Ενημερώνω τον αριθμό των καταστάσεων που χρειάστηκε να εξερευνήσω.
        statesExpanded++;

        // Βρέθηκε κατάσταση που ταυτίζεται με την τελική. Η αναζήτηση τερμάτισε με επιτυχία. Επιστρέφω πίνακα με το μονοπάτι προς την λύση.
        if (*currentElement == *goal){
            // Αποδεσμεύω την μνήμη που δεσμεύεται και δεν χρειάζεται πια.
            while (!frontier.empty()){
                delete frontier.top();
                frontier.pop();
            }

            std::vector<Puzzle*> path = currentElement->pathToRoot();

            for (Puzzle *p : path){ // Αφαιρώ απο το κλειστό σύνολο τις καταστάσεις του μονοπατιού που δεν θέλω να διαγραφούν.
                if (closedSet.count(p)!=0){
                    closedSet.erase(p);
                }
            }

            for (Puzzle *p : closedSet){ // Διαγράφω τις καταστάσεις του κλειστού συνόλου που δεν οδήγησαν στην λύση
                delete p;
            }

            return path;
        }

        // Επεκτείνω (εφαρμόζω τους τελεστές μετάβασης για) την τρέχουσα κατάσταση και προσθέτω τα παιδιά της στην αρχή του μετώπου αναζήτησης.
        for (Puzzle *ptr : currentElement->expandAll()){
            ptr->setHeuristicEvaluation(ptr->tilesOutOfPlace(*goal)); // Εφαρμόζω ευρετική συνάρτηση για κάθε παιδί.
            frontier.push(ptr);
        }

        // Βάζω την κατάσταση γονέα στο κλειστό σύνολο.
        closedSet.insert(currentElement);
    }

    // Αποδεσμεύω την μνήμη που δεσμεύεται και δεν χρειάζεται πια.
    while (!frontier.empty()){
        delete frontier.top();
        frontier.pop();
    }
    for (Puzzle *p : closedSet){ // Διαγράφω τις καταστάσεις του κλειστού συνόλου που δεν οδήγησαν στην λύση
        delete p;
    }
    return std::vector<Puzzle *>(); // Τερματισμός με αποτυχία, επιστρέφω κενό πίνακα.
}


/*
 * Ψάχνει λύση στο πρόβλημα με πλακίδια απο την αρχική κατάσταση initial στην τελική goal χρησιμοποιώντας τον αλγόριθμο ευρετικής αναζήτησης
 * Άλφα-Άστρο / A Star / A*.
 *
 * Αν η λύση βρεθεί, επιστρέφεται πίνακας με δείκτες των καταστάσεων του μονοπατιού (ακολουθία καταστάσεων) απο την αρχική μέχρι την τελική.
 * Αν η λύση δεν βρεθεί επιστρέφεται κενός πίνακας.
 *
 * Η παράμετρος statesExpanded θέτει ως τιμή τον αριθμό των καταστάσεων που εξερευνήθηκαν.
 * Η παράμετρος maxTotalStatesInMemory θέτει ως τιμή τον μέγιστο συνολικό αριθμό των καταστάσεων που υπήρξαν σε κάποιο σημείο εκτέλεσης του αλγορίθμου.
 */
std::vector<Puzzle *> A_Star(Puzzle *initial, Puzzle *goal, unsigned long &statesExpanded, unsigned long &maxTotalStatesInMemory){
    std::priority_queue<Puzzle *, std::deque<Puzzle *>, PuzzleGreaterHeuristicValueComparator> frontier; // Ορίζω την δομή που αντιστοιχεί στο μέτωπο αναζήτησης. Ουρά προτεραιότητας για τον A*.
                                                                                                         // Ως συνάρτηση σύκρισης ορίζεται η PuzzleGreaterHeuristicValueComparator.
    std::unordered_set<Puzzle*, PuzzleHashFunction, PuzzlePtrEquals> closedSet; // Ορίζω την δομή που αντιστοιχεί στο κλειστό σύνολο.
                                                                                // Ως συνάρτηση κατακερματισμού ορίζεται η PuzzleHashFunction και συνάρτηση ισότητας η PuzzlePtrEquals.

    // Ορίζω αρχικές τιμές
    statesExpanded = 0;
    maxTotalStatesInMemory = 0;

    frontier.push(initial); // Εισάγω στο μέτωπο την αρχική κατάσταση.

    Puzzle * currentElement; // Ορίζω δείκτη για το τρέχον στοιχείο που εξετάζεται σε εκάστοτε επανάληψη.

    while (!frontier.empty()){ // Όταν αδειάσει το μέτωπο αναζήτησης, η αναζήτηση λύσης έχει τελειώσει με αποτυχία.
        // Ενημερώνω την μεταβλητή μνήμης.
        if (frontier.size()+closedSet.size() > maxTotalStatesInMemory){
            maxTotalStatesInMemory = frontier.size() + closedSet.size();
        }

        currentElement = frontier.top(); // Παίρνω το πρώτο στοιχείο απο το μέτωπο αναζήτησης.
        frontier.pop(); // Αφαιρώ το στοιχείο που μόλις πήρα.

        if (closedSet.count(currentElement) != 0){ // Αν η κατάσταση υπάρχει στο κλείστο σύνολο (την έχω ξαναεπισκευτεί), την αγνοώ.
            delete currentElement;
            continue;
        }

        // Ενημερώνω τον αριθμό των καταστάσεων που χρειάστηκε να εξερευνήσω.
        statesExpanded++;

        // Βρέθηκε κατάσταση που ταυτίζεται με την τελική. Η αναζήτηση τερμάτισε με επιτυχία. Επιστρέφω πίνακα με το μονοπάτι προς την λύση.
        if (*currentElement == *goal){
            // Αποδεσμεύω την μνήμη που δεσμεύεται και δεν χρειάζεται πια.
            while (!frontier.empty()){
                delete frontier.top();
                frontier.pop();
            }

            std::vector<Puzzle*> path = currentElement->pathToRoot();

            for (Puzzle *p : path){ // Αφαιρώ απο το κλειστό σύνολο τις καταστάσεις του μονοπατιού που δεν θέλω να διαγραφούν.
                if (closedSet.count(p)!=0){
                    closedSet.erase(p);
                }
            }

            for (Puzzle *p : closedSet){ // Διαγράφω τις καταστάσεις του κλειστού συνόλου που δεν οδήγησαν στην λύση
                delete p;
            }

            return path;
        }

        // Επεκτείνω (εφαρμόζω τους τελεστές μετάβασης για) την τρέχουσα κατάσταση και προσθέτω τα παιδιά της στην αρχή του μετώπου αναζήτησης.
        for (Puzzle *ptr : currentElement->expandAll()){
            // Εφαρμόζω ευρετική συνάρτηση για κάθε παιδί.
            // Στον A*, ως ευρετική συνάρτηση χρησιμοποιούμε το άθροισμα της τυπικής (εκείνης που θα χρησιμοποιούσε ο BestFS) ευρετικής συνάρτησης με το μήκος του μονοπατιού μέχρι τον εκάστοτε κόμβο.
            ptr->setHeuristicEvaluation(ptr->tilesOutOfPlace(*goal) + ptr->getLengthToRoot());
            frontier.push(ptr);
        }

        // Βάζω την κατάσταση γονέα στο κλειστό σύνολο.
        closedSet.insert(currentElement);
    }

    // Αποδεσμεύω την μνήμη που δεσμεύεται και δεν χρειάζεται πια.
    while (!frontier.empty()){
        delete frontier.top();
        frontier.pop();
    }
    for (Puzzle *p : closedSet){ // Διαγράφω τις καταστάσεις του κλειστού συνόλου που δεν οδήγησαν στην λύση
        delete p;
    }
    return std::vector<Puzzle *>(); // Τερματισμός με αποτυχία, επιστρέφω κενό πίνακα.
}

int main() {
    // Ορίζω και κατασκευάζω την αρχική κατάσταση.
    Puzzle * initialPuzzleState = new Puzzle();
    std::vector<std::vector<unsigned long>> tileValues;
    std::vector<unsigned long> row;
    row.push_back(3);
    row.push_back(6);
    row.push_back(0);
    tileValues.push_back(row); row.clear();
    row.push_back(1);
    row.push_back(4);
    row.push_back(2);
    tileValues.push_back(row); row.clear();
    row.push_back(7);
    row.push_back(5);
    row.push_back(8);
    tileValues.push_back(row);
    initialPuzzleState->setTileValues(tileValues);

    // Ορίζω και κατασκευάζω την τελική κατάσταση.
    Puzzle * finalPuzzleState = new Puzzle();
    tileValues.clear();
    row.push_back(1);
    row.push_back(2);
    row.push_back(3);
    tileValues.push_back(row); row.clear();
    row.push_back(4);
    row.push_back(5);
    row.push_back(6);
    tileValues.push_back(row); row.clear();
    row.push_back(7);
    row.push_back(8);
    row.push_back(0);
    tileValues.push_back(row);
    finalPuzzleState->setTileValues(tileValues);



    // Μεταβλητές στατιστικών.
    unsigned long explored, maxMemoryUsed;

    // Πίνακας με το μονοπάτι της λύσης.
    std::vector<Puzzle*> solution;

    /* Εύρεση λύσης με τον Depth First Search. */

    auto searchStartTime = std::chrono::steady_clock::now(); // Αρχίζω το χρονόμετρο
    solution = DFS(initialPuzzleState, finalPuzzleState, explored, maxMemoryUsed);
    auto searchEndTime = std::chrono::steady_clock::now(); // Σταματάω το χρονόμετρο

    if (solution.size() != 0) { // Βρέθηκε λύση.
        // Εκτύπωση στατιστικών αποτελεσμάτων.
        std::cout << "---- Depth First Search ----" << std::endl
                  << "Solution path length: " << solution.back()->getLengthToRoot()
                  << ", Number of states explored (expanded): " << explored
                  << ", Maximum number of states in memory at once: " << maxMemoryUsed
                  << ", Search duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count() * 0.000001 << " ms" << std::endl << std::endl;


        /* // Εκτύπωση λύσης. Βρίσκεται σε σχόλια καθώς ενδεχομένως έχει πολύ μεγάλο μήκος.
        std::cout << "Moves to reach solution (goal state): " << std::endl;
        for (Puzzle *p : solution){
            std::cout << p->getParentAction() << std::endl;
        }
        std::cout << std::endl << std::endl;
        */


        // Αποδέσμευση μνήμης, εκτός της αρχικής που θα ξαναχρησιμοποιηθεί και θα αποδεσμευτεί σε ύστερο χρόνο.
        for (unsigned long i = 1; i < solution.size(); i++) {
            delete solution.at(i);
        }
    } else {
        std::cout << "---- Depth First Search ----" << std::endl
                  << "No solution!"
                  << ", Number of states explored (expanded): " << explored
                  << ", Maximum number of states in memory at once: " << maxMemoryUsed
                  << ", Search duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count() * 0.000001 << " ms" << std::endl << std::endl;

    }

    /* Εύρεση λύσης με τον Breadth First Search. */

    searchStartTime = std::chrono::steady_clock::now(); // Αρχίζω το χρονόμετρο
    solution = BFS(initialPuzzleState, finalPuzzleState, explored, maxMemoryUsed);
    searchEndTime = std::chrono::steady_clock::now(); // Σταματάω το χρονόμετρο

    if (solution.size() != 0) { // Βρέθηκε λύση.
        // Εκτύπωση στατιστικών αποτελεσμάτων.
        std::cout << "---- Breadth First Search ----" << std::endl
                  << "Solution path length: " << solution.back()->getLengthToRoot()
                  << ", Number of states explored (expanded): " << explored
                  << ", Maximum number of states in memory at once: " << maxMemoryUsed
                  << ", Search duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count() * 0.000001 << " ms" << std::endl << std::endl;


        // Εκτύπωση λύσης.
        std::cout << "Moves to reach solution (goal state): " << std::endl;
        for (Puzzle *p : solution) {
            std::cout << p->getParentAction() << std::endl;
        }
        std::cout << std::endl << std::endl;

        // Αποδέσμευση μνήμης, εκτός της αρχικής που θα ξαναχρησιμοποιηθεί και θα αποδεσμευτεί σε ύστερο χρόνο.
        for (unsigned long i = 1; i < solution.size(); i++) {
            delete solution.at(i);
        }
    } else {
        std::cout << "---- Breadth First Search ----" << std::endl
                  << "No solution!"
                  << ", Number of states explored (expanded): " << explored
                  << ", Maximum number of states in memory at once: " << maxMemoryUsed
                  << ", Search duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count() * 0.000001 << " ms" << std::endl << std::endl;

    }

    /* Εύρεση λύσης με τον Best First Search. */

    searchStartTime = std::chrono::steady_clock::now(); // Αρχίζω το χρονόμετρο
    solution = BestFS(initialPuzzleState, finalPuzzleState, explored, maxMemoryUsed);
    searchEndTime = std::chrono::steady_clock::now(); // Σταματάω το χρονόμετρο

    if (solution.size() != 0) { // Βρέθηκε λύση.
        // Εκτύπωση στατιστικών αποτελεσμάτων.
        std::cout << "---- Best First Search ----" << std::endl
                  << "Solution path length: " << solution.back()->getLengthToRoot()
                  << ", Number of states explored (expanded): " << explored
                  << ", Maximum number of states in memory at once: " << maxMemoryUsed
                  << ", Search duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count() * 0.000001 << " ms" << std::endl << std::endl;

        /* // Εκτύπωση λύσης. Βρίσκεται σε σχόλια καθώς ενδεχομένως έχει πολύ μεγάλο μήκος.
        std::cout << "Moves to reach solution (goal state): " << std::endl;
        for (Puzzle *p : solution){
            std::cout << p->getParentAction() << std::endl;
        }
        std::cout << std::endl << std::endl;
        */


        // Αποδέσμευση μνήμης, εκτός της αρχικής που θα ξαναχρησιμοποιηθεί και θα αποδεσμευτεί σε ύστερο χρόνο.
        for (unsigned long i = 1; i < solution.size(); i++) {
            delete solution.at(i);
        }
    } else {
        std::cout << "---- Best First Search ----" << std::endl
                  << "No solution!"
                  << ", Number of states explored (expanded): " << explored
                  << ", Maximum number of states in memory at once: " << maxMemoryUsed
                  << ", Search duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count() * 0.000001 << " ms" << std::endl << std::endl;
    }

    /* Εύρεση λύσης με τον A*. */

    searchStartTime = std::chrono::steady_clock::now(); // Αρχίζω το χρονόμετρο
    solution = A_Star(initialPuzzleState, finalPuzzleState, explored, maxMemoryUsed);
    searchEndTime = std::chrono::steady_clock::now(); // Σταματάω το χρονόμετρο

    if (solution.size() != 0) { // Βρέθηκε λύση.
        // Εκτύπωση στατιστικών αποτελεσμάτων.
        std::cout << "---- A* ----" << std::endl
                  << "Solution path length: " << solution.back()->getLengthToRoot()
                  << ", Number of states explored (expanded): " << explored
                  << ", Maximum number of states in memory at once: " << maxMemoryUsed
                  << ", Search duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count() * 0.000001 << " ms" << std::endl << std::endl;

        // Εκτύπωση λύσης.
        std::cout << "Moves to reach solution (goal state): " << std::endl;
        for (Puzzle *p : solution) {
            std::cout << p->getParentAction() << std::endl;
        }

        std::cout << std::endl << std::endl;

        // Αποδέσμευση μνήμης, εκτός της αρχικής που θα αποδεσμευτεί σε ύστερο χρόνο.
        for (unsigned long i = 1; i < solution.size(); i++) {
            delete solution.at(i);
        }
    } else {
        std::cout << "---- A* ----" << std::endl
                  << "No solution!"
                  << ", Number of states explored (expanded): " << explored
                  << ", Maximum number of states in memory at once: " << maxMemoryUsed
                  << ", Search duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count() * 0.000001 << " ms" << std::endl << std::endl;

    }



    delete initialPuzzleState;
    delete finalPuzzleState;



    return 0;
}
