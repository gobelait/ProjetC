#include "Interpreteur.h"
#include <stdlib.h>
#include <iostream>
#include <typeinfo>

using namespace std;

Interpreteur::Interpreteur(ifstream & fichier) :
m_lecteur(fichier), m_table(), m_arbre(nullptr) {
}

void Interpreteur::analyse() {
    m_erreur = 0;
    m_arbre = programme(); // on lance l'analyse de la première règle

    if(m_erreur>0){
        m_arbre = nullptr;
        exit(EXIT_FAILURE);
    }
}

void Interpreteur::tester(const string & symboleAttendu) const throw (SyntaxeException) {
  // Teste si le symbole courant est égal au symboleAttendu... Si non, lève une exception
  static char messageWhat[256];
  if (m_lecteur.getSymbole() != symboleAttendu) {
    sprintf(messageWhat,
            "Ligne %d, Colonne %d - Erreur de syntaxe - Symbole attendu : %s - Symbole trouvé : %s",
            m_lecteur.getLigne(), m_lecteur.getColonne(),
            symboleAttendu.c_str(), m_lecteur.getSymbole().getChaine().c_str());
    throw SyntaxeException(messageWhat);
  }
}

void Interpreteur::testerEtAvancer(const string & symboleAttendu) throw (SyntaxeException) {
  // Teste si le symbole courant est égal au symboleAttendu... Si oui, avance, Sinon, lève une exception
  tester(symboleAttendu);
  m_lecteur.avancer();
}

void Interpreteur::erreur(const string & message) const throw (SyntaxeException) {
  // Lève une exception contenant le message et le symbole courant trouvé
  // Utilisé lorsqu'il y a plusieurs symboles attendus possibles...
  static char messageWhat[256];
  sprintf(messageWhat,
          "Ligne %d, Colonne %d - Erreur de syntaxe - %s - Symbole trouvé : %s",
          m_lecteur.getLigne(), m_lecteur.getColonne(), message.c_str(), m_lecteur.getSymbole().getChaine().c_str());
  throw SyntaxeException(messageWhat);
}

Noeud* Interpreteur::programme() {
  // <programme> ::= procedure principale() <seqInst> finproc FIN_FICHIER
  testerEtAvancer("procedure");
  testerEtAvancer("principale");
  testerEtAvancer("(");
  testerEtAvancer(")");
  Noeud* sequence = seqInst();
  testerEtAvancer("finproc");
  tester("<FINDEFICHIER>");
  return sequence;
}

Noeud* Interpreteur::seqInst() {
  // <seqInst> ::= <inst> { <inst> }
  NoeudSeqInst* sequence = new NoeudSeqInst();
  do {
    sequence->ajoute(inst());
  } while (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "si" || m_lecteur.getSymbole() == "tantque" || m_lecteur.getSymbole() == "repeter" || m_lecteur.getSymbole() == "pour" || m_lecteur.getSymbole() == "ecrire" || m_lecteur.getSymbole() == "lire" /*|| m_lecteur.getSymbole() == "selon"*/);
  // Tant que le symbole courant est un début possible d'instruction...
  // Il faut compléter cette condition chaque fois qu'on rajoute une nouvelle instruction
  return sequence;
}

Noeud* Interpreteur::inst() {
  // <inst> ::= <affectation>  ; | <instSiRiche> || <instTantQue> || <instRepeter> || <>
    try{
   if (m_lecteur.getSymbole() == "<VARIABLE>") {
    Noeud *affect = affectation();
    testerEtAvancer(";");
    return affect;
  }
  else if (m_lecteur.getSymbole() == "si")
    return instSiRiche();
  else if (m_lecteur.getSymbole() == "sinonsi")
      return instSiRiche();
  else if (m_lecteur.getSymbole() == "sinon")
      return instSiRiche();
  else if (m_lecteur.getSymbole() == "repeter")
      return instRepeter();
  else if (m_lecteur.getSymbole() == "tantque")
      return instTantQue();
  else if (m_lecteur.getSymbole() == "pour")
      return instPour();
  else if (m_lecteur.getSymbole() == "ecrire")
      return instEcrire();
  else if (m_lecteur.getSymbole() == "lire")
      return instLire();
//  else if (m_lecteur.getSymbole() == "selon")
//      return instSelon();
  // Compléter les alternatives chaque fois qu'on rajoute une nouvelle instruction
  else erreur("Instruction incorrecte");

    }catch(SyntaxeException & e){
        cout << e.what() << endl;
        m_erreur++;
        while(!(m_lecteur.getSymbole() == "finproc" || m_lecteur.getSymbole() == "<FINDEFICHIER>" || m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "si" || m_lecteur.getSymbole() == "sinonsi" || m_lecteur.getSymbole() == "repeter" || m_lecteur.getSymbole() == "tantque" || m_lecteur.getSymbole() == "pour" || m_lecteur.getSymbole() == "ecrire" || m_lecteur.getSymbole() == "lire")){
            m_lecteur.avancer();
        }   
    }
}

Noeud* Interpreteur::affectation() {
  // <affectation> ::= <variable> = <expression> 
  tester("<VARIABLE>");
  Noeud* var = m_table.chercheAjoute(m_lecteur.getSymbole()); // La variable est ajoutée à la table et on la mémorise
  m_lecteur.avancer();
  if(m_lecteur.getSymbole() == "="){
    testerEtAvancer("=");
    Noeud* exp = expression();             // On mémorise l'expression trouvée
    return new NoeudAffectation(var, exp); // On renvoie un noeud affectation     
  }
  else if(m_lecteur.getSymbole() == "++"){
    testerEtAvancer("++");
    return new NoeudIncr(var);
  }
  else if(m_lecteur.getSymbole() == "--"){
    testerEtAvancer("--");
    return new NoeudDecr(var);
  }
  else if(m_lecteur.getSymbole() == "-"){
    testerEtAvancer("-");
    testerEtAvancer("=");
    Noeud* expressionDroite = expression();
    return new NoeudAffectationDirecteSoustraction(var,expressionDroite);
  }
  else if(m_lecteur.getSymbole() == "+"){
    testerEtAvancer("+");
    testerEtAvancer("=");
    Noeud* expressionDroite = expression();
    return new NoeudAffectationDirecteAddition(var,expressionDroite);
  }
  else if(m_lecteur.getSymbole() == "*"){
    testerEtAvancer("*");
    testerEtAvancer("=");
    Noeud* expressionDroite = expression();
    return new NoeudAffectationDirecteMultiplication(var,expressionDroite);
  }
  else if(m_lecteur.getSymbole() == "/"){
    testerEtAvancer("/");
    testerEtAvancer("=");
    Noeud* expressionDroite = expression();
    return new NoeudAffectationDirecteDivision(var,expressionDroite);
  }

}

Noeud* Interpreteur::expression() {
    // <expression> ::= <terme> { + <terme> | - <terme> }
    Noeud* unTerme = terme();
    while ( m_lecteur.getSymbole() == "+"  || m_lecteur.getSymbole() == "-") {
        Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
        m_lecteur.avancer();
        Noeud* termeDroit = terme(); // On mémorise l'opérande droit
        unTerme = new NoeudOperateurBinaire(operateur, unTerme, termeDroit); // Et on construit un noeud opérateur binaire
    }
    return unTerme; // On renvoie unTerme qui pointe sur la racine de l'expression
}

Noeud* Interpreteur::terme(){
    // <terme> ::= <facteur> {* <facteur> | / <facteur>}
    Noeud* unFacteur = facteur();
    while(m_lecteur.getSymbole() == "*" || m_lecteur.getSymbole() == "/"){
        Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
        m_lecteur.avancer();
        Noeud* facteurDroit = facteur(); //On mémorise l'opérande droit
        unFacteur = new NoeudOperateurBinaire(operateur, unFacteur, facteurDroit); // Et on construit un noeud opérateur binaire
    }
    return unFacteur; // On renvoie unFacteur qui pointe sur la racine du terme
}



Noeud* Interpreteur::facteur() {
    // <facteur> ::= <entier> | <variable> | - <expBool> | non <expBool> | ( <expBool> ) 
    Noeud* fact = nullptr;
    if (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "<ENTIER>") {
        fact = m_table.chercheAjoute(m_lecteur.getSymbole()); // on ajoute la variable ou l'entier à la table
        m_lecteur.avancer();
     } else if (m_lecteur.getSymbole() == "-") { // - <expBool>
     m_lecteur.avancer();
        // on représente le moins unaire (- facteur) par une soustraction binaire (0 - facteur)
        fact = new NoeudOperateurBinaire(Symbole("-"), m_table.chercheAjoute(Symbole("0")), expBool());
    } else if (m_lecteur.getSymbole() == "non") { // non <expBool>
        m_lecteur.avancer();
        // on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
        fact = new NoeudOperateurBinaire(Symbole("non"), expBool(), nullptr);
    } else if (m_lecteur.getSymbole() == "(") { // expBool parenthésée
        m_lecteur.avancer();
        fact = expBool();
        testerEtAvancer(")");
     } else
        erreur("Facteur incorrect");
    return fact;
}

Noeud* Interpreteur::expBool(){
    // <expBool>  ::= <relationET> { ou <relationEt> }
    Noeud* relation = relationEt();
    while ( m_lecteur.getSymbole() == "ou"){
        Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
        m_lecteur.avancer();
        Noeud* relationDroit = relationEt(); //On mémorise l'opérande droit
        relation = new NoeudOperateurBinaire(operateur, relation, relationDroit); // Et on construit un noeud opérateur binaire
    }
    return relation; // On renvoie relation qui pointe sur la racine de l'expression Booléenne
}

Noeud* Interpreteur::relationEt(){
    // <relationEt>  ::= <relation> { et <relation> }
    Noeud* relation2 = relation();
    while ( m_lecteur.getSymbole() == "et"){
        Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
        m_lecteur.avancer();
        Noeud* relationDroit = relation(); //On mémorise l'opérande droit
        relation2 = new NoeudOperateurBinaire(operateur, relation2, relationDroit); // Et on construit un noeud opérateur binaire
    }
    return relation2; // On renvoie relation2 qui pointe sur la racine de la relationEt
}

Noeud* Interpreteur::relation(){
    //<relation> ::= <expression> { <opRel> <expression> }
    Noeud* expression2 = expression();
    while ( m_lecteur.getSymbole() == "<"  || m_lecteur.getSymbole() == "<=" ||
        m_lecteur.getSymbole() == ">"  || m_lecteur.getSymbole() == ">=" ||
        m_lecteur.getSymbole() == "==" || m_lecteur.getSymbole() == "!="){
        Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
        m_lecteur.avancer();
        Noeud* expressionDroit = expression(); //On mémorise l'opérande droit
        expression2 = new NoeudOperateurBinaire(operateur, expression2, expressionDroit); // Et on construit un noeud opérateur binaire
    }
    return expression2; // On renvoie expression2 qui pointe sur la racine de la relation         
}



 Noeud* Interpreteur::instTantQue(){
// <instTantQue> ::= tantque ( <expression> ) <seqInst> fintantque
    testerEtAvancer("tantque");
    testerEtAvancer("(");
    Noeud* expressionTQ = expBool();
    testerEtAvancer(")");
    Noeud* sequence = seqInst();
    testerEtAvancer("fintantque");
    return new NoeudInstTantQue(expressionTQ,sequence);
}

Noeud* Interpreteur::instSiRiche() {    
    // <instSiRiche> ::= si(<expression>) <seqInst> {sinonsi (<expression>) <seqInst> } [sinon <seqInst>] finsi
    Noeud* n = new NoeudInstSiRiche();
    testerEtAvancer("si");
    testerEtAvancer("(");
    Noeud* conditionSi = expBool();             //on récupère la condition du si
    n->ajoute(conditionSi);                     // on l'ajoute à notre vecteur
    testerEtAvancer(")");
    Noeud* sequenceSi = seqInst();              // on récupère la sequence du si
    n->ajoute(sequenceSi);                      // on l'ajoute à notre vecteur
    while(m_lecteur.getSymbole()=="sinonsi"){   //tant qu'il y a des "sinonsi"
        testerEtAvancer("sinonsi");
        testerEtAvancer("(");
        Noeud* conditionSinonSi = expBool();    // on récupère la condition du sinonsi
        n->ajoute(conditionSinonSi);            // on l'ajoute à notre vecteur
        testerEtAvancer(")");
        Noeud* sequenceSinonSi = seqInst();     // on récupère la sequence du sinonsi
        n->ajoute(sequenceSinonSi);             // on l'ajoute à notre vecteur
    }
    if(m_lecteur.getSymbole() =="sinon"){
        testerEtAvancer("sinon");
        Noeud* conditionTrue = m_table.chercheAjoute(Symbole("1"));     // on créer une condition vraie pour le sinon
        n->ajoute(conditionTrue);               // on l'ajoute à notre vecteur
        Noeud* sequenceSinon = seqInst();       // on récupère la sequence du sinonsi
        n->ajoute(sequenceSinon);               // on l'ajoute à notre vecteur
    }
    testerEtAvancer("finsi");
    return n;
}

Noeud* Interpreteur::instRepeter() {
    // <instRepeter> ::= repeter <seqInst> jusqua ( <expression> )
    testerEtAvancer("repeter");
    Noeud* sequence = seqInst();    // on récupère la sequence du repeter
    testerEtAvancer("jusqua");
    testerEtAvancer("(");
    Noeud* finBoucle = expBool();   // on récupère la condition de sortie
    testerEtAvancer(")");
    return new NoeudInstRepeter(sequence, finBoucle);
}

Noeud* Interpreteur::instPour(){
    
    testerEtAvancer("pour");
    testerEtAvancer("(");
    
    Noeud* affectation1 = nullptr;
    if (m_lecteur.getSymbole() == "<VARIABLE>") {       // on récupère la première affectation, si il n'y en a pas, notre noeud vaudra nullptr
        affectation1 = affectation();
    }
    
    testerEtAvancer(";");
    Noeud* expression1 = expBool();                     // on récupère la condition du pour
    testerEtAvancer(";");
    
    Noeud* affectation2 = nullptr;
    if (m_lecteur.getSymbole() == "<VARIABLE>") {       // on récupère la deuxième affectation, si il n'y en a pas, notre noeud vaudra nullptr
        affectation2 = affectation();
    }

    testerEtAvancer(")");
    Noeud* seq = seqInst();                             // on récupère la sequence du pour
    testerEtAvancer("finpour");
    Noeud* n = new NoeudInstPour(affectation1,expression1,affectation2,seq);
    return n;
}

Noeud* Interpreteur::instEcrire(){
    testerEtAvancer("ecrire");
    tester("(");
    Noeud* n = new NoeudInstEcrire();
    do{
        m_lecteur.avancer();
        if(m_lecteur.getSymbole() == "<CHAINE>"){       // si l'élement à afficher est une chaine
            n->ajoute(m_table.chercheAjoute(m_lecteur.getSymbole()));   // on l'ajoute à la table
            m_lecteur.avancer();
        }
        else{
            n->ajoute(expression());                    // sinon, on récupère une expression
        }
    }while(m_lecteur.getSymbole() == ",");              // tant qu'il y a des éléments à ecrire
    testerEtAvancer(")");
    return n;
}

Noeud* Interpreteur::instLire(){
    testerEtAvancer("lire");
    testerEtAvancer("(");
    Noeud* n = new NoeudInstLire;
    do{
        tester("<VARIABLE>");
        n->ajoute(m_table.chercheAjoute(m_lecteur.getSymbole()));       // on ajoute la variable à la table
        m_lecteur.avancer();
        if(m_lecteur.getSymbole() != ")") {             
            testerEtAvancer(",");
        }
    }while(m_lecteur.getSymbole() != ")");              // tant qu'il y a des variables à lire
    testerEtAvancer(")");
    return n;
}

//Noeud* Interpreteur::instSelon(){
//    testerEtAvancer("selon");
//    testerEtAvancer("(");
//    tester("<VARIABLES>");
//    Noeud* variable = m_table.chercheAjoute(m_lecteur.getSymbole());
//    m_lecteur.avancer();
//    NoeudInstSelon instSelon = NoeudInstSelon(variable);
//    while (m_lecteur.getSymbole() == "cas"){
//        m_lecteur.avancer();
//        Noeud* valeurAttendu =  m_table.chercheAjoute(m_lecteur.getSymbole());
//        m_lecteur.avancer();
//        instSelon.ajoute(valeurAttendu);
//        Noeud* sequence = seqInst();
//        instSelon.ajoute(sequence);
//    }
//    testerEtAvancer("finselon");
//    return instSelon;
//    
//}

void Interpreteur::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    cout << setw(4*indentation) << "" << "int main() {" << endl; //Début d'un programme C++
    for (int i=0; i < m_table.getTaille(); i++){
        if (m_table[i] == "<VARIABLE>"){
            cout <<setw((4*indentation)+1)<< "" <<"int " << m_table[i].getChaine() << ";" << endl;  // on écrit les déclarations des variables
        }
    }
    getArbre()->traduitEnCPP(cout,(4*indentation)+1); // lance l'opération traduitEnCpp sur la racine
    cout << endl << setw((4*indentation)+1) << "" << "return 0;" << endl;
    cout << setw(4*indentation) << "}" << endl; //Fin d'un programme C++
}

