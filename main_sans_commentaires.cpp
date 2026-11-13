#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <ctime>

using namespace std;

void viderBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string dateDuJour() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%d/%m/%Y", now);
    return string(buf);
}

class Passager {
private:
    int id;
    string nom, prenom, telephone, email;
public:
    Passager() : id(0) {}
    Passager(int id, string nom, string prenom, string tel, string email)
        : id(id), nom(nom), prenom(prenom), telephone(tel), email(email) {}

    int getId() const { return id; }
    string getNom() const { return nom; }
    string getPrenom() const { return prenom; }
    string getTelephone() const { return telephone; }
    string getEmail() const { return email; }

    void afficher() const {
        cout << "  ID: " << id << " | " << nom << " " << prenom
             << " | Tel: " << telephone << " | Email: " << email << endl;
    }
};

class Vol {
private:
    string numero, villeDepart, villeArrivee, dateDepart, heureDepart;
    int capacite;
    double prix;
public:
    Vol() : capacite(0), prix(0) {}
    Vol(string num, string vd, string va, string dd, string hd, int cap, double p)
        : numero(num), villeDepart(vd), villeArrivee(va),
          dateDepart(dd), heureDepart(hd), capacite(cap), prix(p) {}

    string getNumero() const { return numero; }
    string getVilleDepart() const { return villeDepart; }
    string getVilleArrivee() const { return villeArrivee; }
    string getDateDepart() const { return dateDepart; }
    string getHeureDepart() const { return heureDepart; }
    int getCapacite() const { return capacite; }
    double getPrix() const { return prix; }

    void afficher() const {
        cout << "  Vol " << numero << " : " << villeDepart << " -> " << villeArrivee
             << " | " << dateDepart << " " << heureDepart
             << " | Capacite: " << capacite
             << " | Prix: " << fixed << setprecision(0) << prix << " FCFA" << endl;
    }
};

class Reservation {
private:
    int id;
    Passager passager;
    Vol vol;
    string statut;
    string dateReservation;
    double montantPaye;
    bool rembourse;
public:
    Reservation() : id(0), montantPaye(0), rembourse(false) {}
    Reservation(int id, const Passager& p, const Vol& v)
        : id(id), passager(p), vol(v), statut("En attente"),
          dateReservation(dateDuJour()), montantPaye(0), rembourse(false) {}

    int getId() const { return id; }
    Passager getPassager() const { return passager; }
    Vol getVol() const { return vol; }
    string getStatut() const { return statut; }
    string getDateReservation() const { return dateReservation; }
    double getMontantPaye() const { return montantPaye; }
    bool isRembourse() const { return rembourse; }

    void setStatut(const string& s) { statut = s; }
    void setMontantPaye(double m) { montantPaye = m; }
    void setRembourse(bool r) { rembourse = r; }

    void afficher() const {
        cout << "  Reservation #" << id << " | Statut: " << statut << endl;
        cout << "    Passager: " << passager.getNom() << " " << passager.getPrenom()
             << " (ID: " << passager.getId() << ")" << endl;
        cout << "    Vol: " << vol.getNumero() << " " << vol.getVilleDepart()
             << " -> " << vol.getVilleArrivee()
             << " le " << vol.getDateDepart() << " a " << vol.getHeureDepart() << endl;
        cout << "    Date reservation: " << dateReservation << endl;
        if (montantPaye > 0)
            cout << "    Montant paye: " << fixed << setprecision(0) << montantPaye << " FCFA";
        if (rembourse) cout << " | REMBOURSE";
        cout << endl;
    }
};

class Agence {
private:
    vector<Passager> passagers;
    vector<Vol> vols;
    vector<Reservation> reservations;
    int nextPassagerId, nextReservationId;

    Passager* trouverPassager(int id) {
        for (auto& p : passagers) if (p.getId() == id) return &p;
        return nullptr;
    }
    Vol* trouverVol(const string& num) {
        for (auto& v : vols) if (v.getNumero() == num) return &v;
        return nullptr;
    }
    Reservation* trouverReservation(int id) {
        for (auto& r : reservations) if (r.getId() == id) return &r;
        return nullptr;
    }
    int compterReservationsActives(const string& numVol) {
        int count = 0;
        for (auto& r : reservations)
            if (r.getVol().getNumero() == numVol && r.getStatut() != "Annulee")
                count++;
        return count;
    }
    bool aDejaReserve(int idPassager, const string& numVol) {
        for (auto& r : reservations)
            if (r.getPassager().getId() == idPassager
                && r.getVol().getNumero() == numVol
                && r.getStatut() != "Annulee")
                return true;
        return false;
    }
public:
    Agence() : nextPassagerId(1), nextReservationId(1) {}

    void enregistrerPassager() {
        cout << "\n--- Enregistrement d'un passager ---" << endl;
        string nom, prenom, tel, email;
        cout << "Nom: ";       getline(cin, nom);
        cout << "Prenom: ";    getline(cin, prenom);
        cout << "Telephone: "; getline(cin, tel);
        cout << "Email: ";     getline(cin, email);
        if (nom.empty() || prenom.empty()) {
            cout << "Erreur: nom et prenom obligatoires." << endl; return;
        }
        passagers.emplace_back(nextPassagerId++, nom, prenom, tel, email);
        cout << "Passager enregistre (ID: " << (nextPassagerId - 1) << ")." << endl;
    }

    void ajouterVol() {
        cout << "\n--- Ajout d'un vol ---" << endl;
        string num, vd, va, dd, hd;
        int cap; double prix;
        cout << "Numero du vol: ";               getline(cin, num);
        if (trouverVol(num)) { cout << "Erreur: numero deja existant." << endl; return; }
        cout << "Ville de depart: ";             getline(cin, vd);
        cout << "Ville d'arrivee: ";             getline(cin, va);
        cout << "Date de depart (JJ/MM/AAAA): "; getline(cin, dd);
        cout << "Heure de depart (HH:MM): ";     getline(cin, hd);
        cout << "Capacite: ";
        if (!(cin >> cap) || cap <= 0) { viderBuffer(); cout << "Capacite invalide." << endl; return; }
        viderBuffer();
        cout << "Prix (FCFA): ";
        if (!(cin >> prix) || prix < 0) { viderBuffer(); cout << "Prix invalide." << endl; return; }
        viderBuffer();
        vols.emplace_back(num, vd, va, dd, hd, cap, prix);
        cout << "Vol ajoute avec succes." << endl;
    }

    void afficherVols() {
        cout << "\n--- Vols disponibles ---" << endl;
        if (vols.empty()) { cout << "Aucun vol enregistre." << endl; return; }
        for (auto& v : vols) {
            v.afficher();
            cout << "    Places restantes: "
                 << (v.getCapacite() - compterReservationsActives(v.getNumero())) << endl;
        }
    }

    void rechercherVol() {
        cout << "\n--- Recherche de vol ---" << endl;
        string vd, va;
        cout << "Ville de depart: ";  getline(cin, vd);
        cout << "Ville d'arrivee: ";  getline(cin, va);
        bool trouve = false;
        for (auto& v : vols) {
            if (v.getVilleDepart() == vd && v.getVilleArrivee() == va) {
                v.afficher();
                cout << "    Places restantes: "
                     << (v.getCapacite() - compterReservationsActives(v.getNumero())) << endl;
                trouve = true;
            }
        }
        if (!trouve) cout << "Aucun vol trouve." << endl;
    }

    void effectuerReservation() {
        cout << "\n--- Nouvelle reservation ---" << endl;
        int idP;
        cout << "ID du passager: ";
        if (!(cin >> idP)) { viderBuffer(); cout << "ID invalide." << endl; return; }
        viderBuffer();
        Passager* p = trouverPassager(idP);
        if (!p) { cout << "Erreur: passager non enregistre." << endl; return; }

        string numVol;
        cout << "Numero du vol: "; getline(cin, numVol);
        Vol* v = trouverVol(numVol);
        if (!v) { cout << "Erreur: vol introuvable." << endl; return; }
        if (compterReservationsActives(numVol) >= v->getCapacite()) {
            cout << "Erreur: vol complet." << endl; return;
        }
        if (aDejaReserve(idP, numVol)) {
            cout << "Erreur: ce passager a deja reserve ce vol." << endl; return;
        }

        reservations.emplace_back(nextReservationId++, *p, *v);
        reservations.back().setStatut("Validee");
        cout << "Reservation effectuee (ID: " << (nextReservationId - 1) << ")." << endl;
        cout << "Place confirmee definitivement." << endl;
    }

    void enregistrerPaiement() {
        cout << "\n--- Enregistrer un paiement ---" << endl;
        int idR;
        cout << "ID reservation: ";
        if (!(cin >> idR)) { viderBuffer(); cout << "ID invalide." << endl; return; }
        viderBuffer();
        Reservation* r = trouverReservation(idR);
        if (!r) { cout << "Erreur: reservation introuvable." << endl; return; }
        if (r->getStatut() == "Annulee") {
            cout << "Erreur: reservation annulee, paiement impossible." << endl; return;
        }
        if (r->getStatut() == "Payee") {
            cout << "Cette reservation est deja payee." << endl; return;
        }
        double montant = r->getVol().getPrix();
        cout << "Montant a payer: " << fixed << setprecision(0) << montant << " FCFA" << endl;
        cout << "Confirmer le paiement ? (o/n): ";
        string rep; getline(cin, rep);
        if (rep != "o" && rep != "O") { cout << "Paiement annule." << endl; return; }
        r->setMontantPaye(montant);
        r->setStatut("Payee");
        cout << "Paiement enregistre avec succes." << endl;
    }

    void annulerReservation() {
        cout << "\n--- Annulation de reservation ---" << endl;
        int idR;
        cout << "ID reservation: ";
        if (!(cin >> idR)) { viderBuffer(); cout << "ID invalide." << endl; return; }
        viderBuffer();
        Reservation* r = trouverReservation(idR);
        if (!r) { cout << "Erreur: reservation introuvable." << endl; return; }
        if (r->getStatut() == "Annulee") { cout << "Deja annulee." << endl; return; }

        bool etaitPayee = (r->getStatut() == "Payee");
        r->setStatut("Annulee");
        cout << "Reservation annulee." << endl;
        if (etaitPayee) {
            cout << "Remboursement prevu de "
                 << fixed << setprecision(0) << r->getMontantPaye() << " FCFA." << endl;
            r->setRembourse(true);
        }
    }

    void consulterReservations() {
        cout << "\n--- Liste des reservations ---" << endl;
        if (reservations.empty()) { cout << "Aucune reservation." << endl; return; }
        for (auto& r : reservations) r.afficher();
    }

    void afficherReservation() {
        cout << "\n--- Details d'une reservation ---" << endl;
        int idR;
        cout << "ID reservation: ";
        if (!(cin >> idR)) { viderBuffer(); cout << "ID invalide." << endl; return; }
        viderBuffer();
        Reservation* r = trouverReservation(idR);
        if (!r) { cout << "Reservation introuvable." << endl; return; }
        r->afficher();
    }
};

int main() {
    Agence agence;
    int choix;
    do {
        cout << "\n========== AGENCE DE VOYAGE - COTONOU ==========" << endl;
        cout << "1. Enregistrer un passager" << endl;
        cout << "2. Ajouter un vol" << endl;
        cout << "3. Afficher les vols disponibles" << endl;
        cout << "4. Rechercher un vol" << endl;
        cout << "5. Effectuer une reservation" << endl;
        cout << "6. Enregistrer un paiement" << endl;
        cout << "7. Annuler une reservation" << endl;
        cout << "8. Consulter les reservations" << endl;
        cout << "9. Afficher une reservation" << endl;
        cout << "0. Quitter" << endl;
        cout << "Votre choix: ";
        if (!(cin >> choix)) { viderBuffer(); choix = -1; continue; }
        viderBuffer();
        switch (choix) {
            case 1: agence.enregistrerPassager(); break;
            case 2: agence.ajouterVol(); break;
            case 3: agence.afficherVols(); break;
            case 4: agence.rechercherVol(); break;
            case 5: agence.effectuerReservation(); break;
            case 6: agence.enregistrerPaiement(); break;
            case 7: agence.annulerReservation(); break;
            case 8: agence.consulterReservations(); break;
            case 9: agence.afficherReservation(); break;
            case 0: cout << "Au revoir !" << endl; break;
            default: cout << "Choix invalide." << endl;
        }
    } while (choix != 0);
    return 0;
}
