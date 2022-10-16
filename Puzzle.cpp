//
// Created by Ioannis Baraklilis on 26/3/21.
// Comments in UTF-8 encoding.
//

#include <unordered_set>
#include "Puzzle.h"

/*
 * Default κατασκευαστής, αρχικοποιεί τις τιμές των όλων πλακιδίων σε πρότυπη κατάσταση, δηλαδή αρχίζοντας απο 1 στο πρώτο
 * στοιχείο στην πρώτη στήλη και πρώτη γραμμή και κάθε επόμενο στοιχείο (επόμενης στήλης) έχει τιμή κατα 1 μεγαλύτερη (επόμενο στοιχείο απο εκείνο
 * στην τελευταία στήλη κάποιας γραμμής θεωρείται το στοιχείο της πρώτης στήλης της επόμενης γραμμής, αν υπάρχει). Εξαίρεση αποτελεί το
 * στοιχείο της τελευταίας γραμμής και στήλης όπου έχει τιμή 0 και θεωρείται ως κενό πλακίδιο.
 */
Puzzle::Puzzle() {
    // Ορίζω τυπικό πρόβλημα πλακιδίων.
    for (long i = 0; i < HEIGHT; i++){
        for (long j = 0; j < WIDTH; j++){
            this->tiles[i][j] = i*WIDTH + j + 1;
        }
    }
    this->tiles[HEIGHT-1][WIDTH-1] = 0;

    // Ορίζω λοιπές μεταβλητές.
    this->parentPtr = nullptr;
    this->moveFromParentAction = "Root";
    this->emptyTileRow = HEIGHT-1;
    this->emptyTileColumn = WIDTH-1;
    this->heuristicEvaluation = 0;
}

/*
 * Τελεστής ανάθεσης. Θέτει ως τιμές του τρέχοντος (this) αντικειμένου τις τιμές του δοθέντος αντικειμένου.
 */
Puzzle &Puzzle::operator=(const Puzzle &copySrs) {
    for (long i = 0; i < HEIGHT; i++){
        for (long j = 0; j < WIDTH; j++){
            this->tiles[i][j] = copySrs.tiles[i][j];
        }
    }

    this->parentPtr = copySrs.parentPtr;
    this->moveFromParentAction = copySrs.moveFromParentAction;
    this->emptyTileRow = copySrs.emptyTileRow;
    this->emptyTileColumn = copySrs.emptyTileColumn;
    this->heuristicEvaluation = copySrs.heuristicEvaluation;

    return *this;
}

/*
 * Επιστρέφει συμβολοσειρά που περιγράφει την μεταβολή απο "γονική" κατάσταση ώστε να παραχθεί η τρέχουσα.
 * Σε περίπτωση που αυτή η κατάσταση δεν προήλθε απο κάποια άλλη επιστρέφεται "Root".
 */
std::string Puzzle::getParentAction() {
    return moveFromParentAction;
}

/*
 * Τελεστής ισότητας.
 * Ως ίσες ορίζονται δύο καταστάσεις αν οι θέσεις όλων των πλακιδίων ταυτίζονται,
 * δηλαδή αν έχουν ίδιους πίνακες πλακιδίων.
 */
bool Puzzle::operator==(const Puzzle &compareToObj) const{
    for (long i = 0; i < HEIGHT; i++){
        for (long j = 0; j < WIDTH; j++){
            // Συγκρίνω την τιμή κάθε πλακιδίου μία προς μία.
            // Αν έστω και μία είναι διαφορετική, τότε τα δύο αντικείμενα είναι διάφορα.
            if (this->tiles[i][j] != compareToObj.tiles[i][j]){
                return false;
            }
        }
    }

    return true;
}

/*
 * Θέτει τις τιμές του δισδιάστατου πίνακα tilesArg ως τιμές πλακιδίων στην τρέχουσα κατάσταση.
 *
 * Το μέγεθος του πίνακα ορίσματος πρέπει να έχει μέγεθος ανάλογο με αυτό που ορίζουν οι σταθερές WIDTH και HEIGHT, δηλαδή
 * να έχει αριθμό γραμμών HEIGHT και στηλών WIDTH. Σε διαφορετική περίπτωση η μέθοδος τερματίζει και επιστρέφεται false, ενώ
 * η κατάσταση του πίνακα πλακιδίων δεν μεταβάλεται.
 *
 * Οι τιμές του πίνακα ορίσματος πρέπει να είναι διαφορετικές μεταξύ τους. Σε διαφορετική περίπτωση η μέθοδος τερματίζει και επιστρέφεται false, ενώ
 * η κατάσταση του πίνακα πλακιδίων δεν μεταβάλεται.
 *
 * Ως κενό πλακίδιο θεωρείται το πλακίδιο με τιμή 0.
 *
 * Επιστρέφει την κατάσταση επιτυχίας.
 */
bool Puzzle::setTileValues(std::vector<std::vector<unsigned long>> tilesArg) {
    if (tilesArg.size() != HEIGHT) return false; // Έλεγχος αν είναι ορθός ο αριθμός γραμμών.

    bool methodSuccess = true; // Αποθηκεύει την κατάσταση επιτυχίας της μεθόδου.
    std::vector<std::vector<unsigned long>> backUp = this->getTiles(); // Αποθηκεύω την παλιά κατάσταση του πίνακα σε περίπτωση που χρειαστεί να την επαναφέρω σε περίπτωση λάθους.

    std::unordered_set<unsigned long> tilesEncountered; // Σύνολο που περιέχει τα στοιχεία που έχουν ήδη εισαχθεί για να γίνει έλεγχος μοναδικής εμφάνισης τους.
    long i, j; // Δείκτες τρέχουσας γραμμής και στήλης.

    for (i = 0; i < HEIGHT; i++) { // Πέρασμα κάθε γραμμής
        if (tilesArg[i].size() != WIDTH || !methodSuccess) {// Έλεγχος αν είναι ορθός ο αριθμός στηλών της τρέχουσας γραμμής ή έχει έχει βρεθεί σφάλμα σε προηγούμενη επανάληψη.
            methodSuccess = false;
            break;
        }

        for (j = 0; j < WIDTH; j++){ // Πέρασμα κάθε στοιχείου γραμμής
            if (tilesEncountered.count(tilesArg[i][j]) != 0){ // Έλεγχος μοναδικότητας στοιχείου.
                methodSuccess = false;
                break;
            }
            tilesEncountered.insert(tilesArg[i][j]);

            tiles[i][j] = tilesArg[i][j];

            if (tilesArg[i][j] == 0) { // Αν η τιμή είναι 0, τότε πρέπει να ενημερώσω τις αποθηκευμένες συντεταγμένες του κενού πλακιδίου.
                emptyTileRow = i;
                emptyTileColumn = j;
            }
        }
    }

    if (methodSuccess){ // Άν τερμάτισε με επιτυχία δεν κάνω καμία ενέργεια.
        return true;
    } else { // Επαναφέρω την αρχική κατάσταση.
        setTileValues(backUp); // Θα είναι σίγουρα επιτυχημένη, καθώς για να είναι λειτουργικό αντικείμενο θα πρέπει η προηγούμενη κατάσταση του να είναι ορθή.
                               // Επίσης, ο κατασκευαστής παράγει ορθές καταστάσεις.
        return false;
    }
}

/*
 * Επιστρέφει το μήκος του μονοπατιού απο την ρίζα (τον πιο παλιό "γονέα") μέχρι το τρέχον.
 */
unsigned long Puzzle::getLengthToRoot() {
    unsigned long length = 0; // Το εκάστοτε τρέχον μήκος.
    Puzzle * temp = parentPtr;

    // Αυξάνω το length όσο ο δείκτης προς γονέα δεν είναι κενός, δηλαδή δεν έχω "διασχίσει" όλο το μονοπάτι ακόμα.
    while (temp != nullptr){
        length++;
        temp = temp->parentPtr;
    }

    return length;
}

/*
 * Επιστρέφει το πλήθος των πλακιδίων εκτός θέσης σε σχέση με το αντικείμενο του ορίσματος.
 * Μπορεί να χρησιμοποιηθεί ως ευρεστική συνάρτηση για χρήση σε ευρετικούς αλγορίθμους.
 */
unsigned long Puzzle::tilesOutOfPlace(const Puzzle &compareToObj) {
    unsigned long counter = 0; // Μετρητής πλακιδίων εκτός θέσης.

    for (long i = 0; i < HEIGHT; i++){
        for (long j = 0; j < WIDTH; j++){
            counter += (this->tiles[i][j] != compareToObj.tiles[i][j]); // Κάθε διαφορά αντιστοιχεί σε τιμή 1, οπότε την προσθέτω στο άθροισμα.
        }
    }

    return counter;
}

/*
 * Επιστρέφει την αποθηκευμένη τιμή της ευρετικής συνάρτησης για το δεδομένο (this) αντικείμενο.
 */
unsigned long Puzzle::getHeuristicEvaluation() const {
    return heuristicEvaluation;
}

/*
 * Ορίζει την αποθηκευμένη τιμή της ευρετικής συνάρτησης για το δεδομένο (this) αντικείμενο.
 */
void Puzzle::setHeuristicEvaluation(unsigned long heuristicEvaluation) {
    this->heuristicEvaluation = heuristicEvaluation;
}

/*
 * Επιστρέφει την κατάσταση που προκύπτει με την μετακίνηση του κενού πλακιδίου προς τα πάνω.
 * Αν απαγορεύεται (δεν ορίζεται) η μετάβαση αυτή, επιστρέφει nullptr.
 */
Puzzle * Puzzle::moveUp() {
    if (this->emptyTileRow - 1 >= 0){ // Αν ορίζεται μετάβαση μετακίνησης κενού προς τα πάνω:
        Puzzle * temp = new Puzzle();
        *temp = *this;

        temp->moveFromParentAction = "Moved " + std::to_string(tiles[temp->emptyTileRow - 1][temp->emptyTileColumn]) + " down";
        temp->swapTiles(temp->emptyTileRow, temp->emptyTileColumn, temp->emptyTileRow - 1, temp->emptyTileColumn);
        temp->emptyTileRow--; // Ενημέρωση θέσης κενού πλακιδίου
        temp->parentPtr = this;

        return temp;
    } else { // Δεν ορίζεται η μετάβαση, επιστρέφω nullptr.
        return nullptr;
    }
}

/*
 * Επιστρέφει την κατάσταση που προκύπτει με την μετακίνηση του κενού πλακιδίου προς τα κάτω.
 * Αν απαγορεύεται (δεν ορίζεται) η μετάβαση αυτή, επιστρέφει nullptr.
 */
Puzzle * Puzzle::moveDown() {
    if (this->emptyTileRow + 1 < HEIGHT){ // Αν ορίζεται μετάβαση μετακίνησης κενού προς τα κάτω:
        Puzzle * temp = new Puzzle();
        *temp = *this;

        temp->moveFromParentAction = "Moved " + std::to_string(tiles[temp->emptyTileRow + 1][temp->emptyTileColumn]) + " up";
        temp->swapTiles(temp->emptyTileRow, temp->emptyTileColumn, temp->emptyTileRow + 1, temp->emptyTileColumn);
        temp->emptyTileRow++; // Ενημέρωση θέσης κενού πλακιδίου
        temp->parentPtr = this;

        return temp;
    } else { // Δεν ορίζεται η μετάβαση, επιστρέφω nullptr.
        return nullptr;
    }
}

/*
 * Επιστρέφει την κατάσταση που προκύπτει με την μετακίνηση του κενού πλακιδίου προς τα αριστερά.
 * Αν απαγορεύεται (δεν ορίζεται) η μετάβαση αυτή, επιστρέφει nullptr.
 */
Puzzle * Puzzle::moveLeft() {
    if (this->emptyTileColumn - 1 >= 0){ // Αν ορίζεται μετάβαση μετακίνησης κενού προς τα αριστερά:
        Puzzle * temp = new Puzzle();
        *temp = *this;

        temp->moveFromParentAction = "Moved " + std::to_string(tiles[temp->emptyTileRow][temp->emptyTileColumn - 1]) + " right";
        temp->swapTiles(temp->emptyTileRow, temp->emptyTileColumn, temp->emptyTileRow, temp->emptyTileColumn - 1);
        temp->emptyTileColumn--; // Ενημέρωση θέσης κενού πλακιδίου
        temp->parentPtr = this;

        return temp;
    } else { // Δεν ορίζεται η μετάβαση, επιστρέφω nullptr.
        return nullptr;
    }
}

/*
 * Επιστρέφει την κατάσταση που προκύπτει με την μετακίνηση του κενού πλακιδίου προς τα δεξιά.
 * Αν απαγορεύεται (δεν ορίζεται) η μετάβαση αυτή, επιστρέφει nullptr.
 */
Puzzle * Puzzle::moveRight() {
    if (this->emptyTileColumn + 1 < WIDTH){ // Αν ορίζεται μετάβαση μετακίνησης κενού προς τα δεξιά:
        Puzzle * temp = new Puzzle();
        *temp = *this;

        temp->moveFromParentAction = "Moved " + std::to_string(tiles[temp->emptyTileRow][temp->emptyTileColumn + 1]) + " left";
        temp->swapTiles(temp->emptyTileRow, temp->emptyTileColumn, temp->emptyTileRow, temp->emptyTileColumn + 1);
        temp->emptyTileColumn++; // Ενημέρωση θέσης κενού πλακιδίου
        temp->parentPtr = this;

        return temp;
    } else { // Δεν ορίζεται η μετάβαση, επιστρέφω nullptr.
        return nullptr;
    }
}

/*
 * Επιστρέφει πίνακα με τους δείκτες προς κάθε κατάσταση που μπορεί να παραχθεί (ορίζεται μετάβαση πρός) απο
 * την τρέχουσα.
 */
std::vector<Puzzle *> Puzzle::expandAll() {
    Puzzle * temp; // Δείκτης που αποθηκεύει προσωρινά τους δείκτες πρίν τοποθετηθούν στον πίνακα.

    std::vector<Puzzle *> tempArray; // Ο πίνακας που θα περιέχει όλες τις επιτρεπτές παράγωγες καταστάσεις.
    tempArray.reserve(4); // Εφόσον το κενό πλακίδιο μπορεί να κινηθεί σε τέσσερις το πολύ κατευθύνσεις, δεσμέυονται 4 καταστάσεις.

    if ((temp = moveLeft()) != nullptr){ // Επιτρέπεται κίνηση προς αριστερά.
        tempArray.push_back(temp);
    }

    if ((temp = moveUp()) != nullptr){ // Επιτρέπεται κίνηση προς πάνω.
        tempArray.push_back(temp);
    }

    if ((temp = moveRight()) != nullptr){ // Επιτρέπεται κίνηση προς δεξιά.
        tempArray.push_back(temp);
    }

    if ((temp = moveDown()) != nullptr){ // Επιτρέπεται κίνηση προς κάτω.
        tempArray.push_back(temp);
    }

    tempArray.shrink_to_fit();

    return tempArray;
}

/*
 * Επιστρέφει πίνακα με δείκτες προς τις καταστάσεις που σχηματίζουν μονοπάτι από την αρχική κατάσταση ("ρίζα" / "root")
 * που οδηγεί στην τρέχουσα.
 */
std::vector<Puzzle *> Puzzle::pathToRoot() {
    unsigned long pathLength = this->getLengthToRoot();
    std::vector<Puzzle *> path(pathLength + 1); // Ορίζω τον πίνακα που θα αποθηκεύει τους δείκτες στα στοιχεία του μονοπατιού.

    Puzzle * temp = this;

    // Μειώνω το pathLength όσο ο δείκτης προς γονέα δεν είναι κενός, δηλαδή δεν έχω "διασχίσει" όλο το μονοπάτι ακόμα.
    // Το μειωμένο pathLength είναι η θέση που θα πρέπει να πάρει το επόμενο στοιχείο στον πίνακα μονοπατιού.
    while (temp != nullptr){
        path.at(pathLength) = temp;
        pathLength--;
        temp = temp->parentPtr;
    }

    return path;
}

/*
 * Ανταλλάσει τα στοιχεία του πίνακα τιμών των πλακιδίων.
 */
void Puzzle::swapTiles(long row1, long column1, long row2, long column2) {
    unsigned long temp = tiles[row1][column1];
    tiles[row1][column1] = tiles[row2][column2];
    tiles[row2][column2] = temp;
}

/*
 * Υπερφόρτωση τελεστή <<.
 * Τυπώνει στην δοθείσα έξοδο τα περιεχόμενα του πίνακα ανα γραμμή (δηλαδή την αναπαράσταση της κατάστασης).
 * Ως πρώτη γραμμή θεωρείται η γραμμή 0 (Row 0).
 */
std::ostream &operator<<(std::ostream &os, const Puzzle &puzzle) {
    for (long i = 0; i < HEIGHT; i++){
        os << "Row " << i << ":";
        for (long j = 0; j < WIDTH; j++){
            os << '\t';
            if (puzzle.tiles[i][j] != 0){
                os << puzzle.tiles[i][j];
            } else {
                os << ' ';
            }
        }
        os << std::endl;
    }

    return os;
}

/*
 * Επιστρέφει δισδιάστατο πίνακα με τις τιμές των πλακιδίων.
 */
std::vector<std::vector<unsigned long>> Puzzle::getTiles() {
    std::vector<std::vector<unsigned long>> rowsVector; // Πίνακας με τις γραμμές.
    rowsVector.reserve(HEIGHT); // Γνωρίζω εκ των προτέρων την μνήμη που χρειάζεται να δεσμευτεί οπότε κάνω την δέσμευση.

    for (long i = 0; i < HEIGHT; i++){
        std::vector<unsigned long> row; // Πίνακας με τα στοιχεία μίας γραμμής.
        row.reserve(WIDTH); // Γνωρίζω εκ των προτέρων την μνήμη που χρειάζεται να δεσμευτεί οπότε κάνω την δέσμευση.

        for (long j = 0; j < WIDTH; j++){
            row.push_back(this->tiles[i][j]);
        }

        rowsVector.push_back(row);
    }

    return rowsVector;
}

/*
 * Αποτελεί συνάρτηση κατακερματισμού της κλάσης.
 */
unsigned long Puzzle::getHashValue() {
    unsigned long temp = 127; // Προσωρινή μεταβλητή που θα αποθηκεύεται η τιμή κατακερματισμού. Το 127 είναι πρώτος αριθμός.

    for (long i = 0; i < HEIGHT; i++){
        for (long j = 0; j < WIDTH; j++){
            temp = (11 * temp + this->tiles[i][j]) % 0xffffffff; // Η συνάρτηση κατακερματιμού ορίζεται ως Στοιχείο πίνακα * 11 ^ σειρά εμφάνισης στοιχείου και σε κάθε βήμα περιορίζεται στο εύρος του unsigned long.
        }
    }

    return temp;

}
