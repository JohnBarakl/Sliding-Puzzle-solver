# Sliding-Puzzle-solver
Υλοποίηση προγράμματος που επιλύει το πρόβλημα με πλακίδια (Sliding Puzzle). <br>
Η υλοποίηση αυτή έγινε στο πλαίσιο εργασίας στο μάθημα "Τεχνητή Νοημοσύνη".

## Περιγραφή προβλήματος:
Το παρακάτω σχήμα παρουσιάζει ένα πρόβλημα με πλακίδια το οποίο αποτελείται από WIDTH στήλες και
HEIGHT γραμμές και σε κάθε κελί υπάρχει ένα διαφορετικό σύμβολο. Ένα από τα κελιά είναι κενό και το
ζητούμενο είναι από την αρχική κατάσταση να φτάσουμε στην τελική μετακινώντας (σέρνοντας) διαδοχικά
γειτονικά πλακίδια στη θέση του κενού κελιού. Π.χ. στην αρχική κατάσταση του παραπάνω σχήματος οι
δυνατές κινήσεις είναι να σύρουμε το πλακίδιο με τον αριθμό «6» προς τα δεξιά και να σύρουμε το
πλακίδιο με τον αριθμό «2» προς τα πάνω.
***
```
╔═══╦═══╦═══╗            ╔═══╦═══╦═══╗ 
║ 3 ║ 6 ║   ║            ║ 1 ║ 2 ║ 3 ║ 
╠═══╬═══╬═══╣            ╠═══╬═══╬═══╣ 
║ 1 ║ 4 ║ 2 ║   ┉┉┉┉┉▶  ║ 4 ║ 5 ║ 6 ║ 
╠═══╬═══╬═══╣            ╠═══╬═══╬═══╣ 
║ 7 ║ 5 ║ 8 ║            ║ 7 ║ 8 ║   ║ 
╚═══╩═══╩═══╝            ╚═══╩═══╩═══╝ 
```
Σχήμα προβλήματος που περιγράφεται παραπάνω.
***

## Απαιτήσεις
Καλείστε να υλοποιήσετε σε C/C++:
* Τη μοντελοποίηση του παραπάνω προβλήματος.
  * Έγινε στα αρχεία Puzzle.h/cpp.
* Τους αλγορίθμους Depth First Search, Breadth First Search, Best First Search και A* για να λύσετε το
  παραπάνω πρόβλημα.
  * Έγινε στο αρχείο main.cpp, όπου οι αλγόριθμοι υλοποιήθηκαν ως συναρτήσεις και η επίλυση του προβλήματος έγινε στη συνάρτηση main.



