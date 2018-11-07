#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"
#include <typeinfo>

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
    for (unsigned int i = 0; i < m_instructions.size(); i++){
        m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
    }
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}


void NoeudSeqInst::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    for (unsigned int i = 0; i < m_instructions.size(); i++){
        m_instructions[i]->traduitEnCPP(cout, indentation);
        if ((typeid(*m_instructions[i])==typeid(NoeudAffectation)) ||
        (typeid(*m_instructions[i])==typeid(NoeudInstRepeter)) ||
        (typeid(*m_instructions[i])==typeid(NoeudInstEcrire)) ||
        (typeid(*m_instructions[i])==typeid(NoeudInstLire))) {
            cout << ";" << endl;
        }   
    }
}




////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
  int valeur = m_expression->executer(); // On exécute (évalue) l'expression
  ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudAffectation::traduitEnCPP(ostream& cout, unsigned int indentation) const{
//    cout << setw(indentation) << ((SymboleValue*)m_variable)->getChaine();
    m_variable->traduitEnCPP(cout, indentation);
    cout << "=";
    m_expression->traduitEnCPP(cout, 0);
}





////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
  int og, od, valeur;
  if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
  if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
  // Et on combine les deux opérandes en fonctions de l'opérateur
  if (this->m_operateur == "+") valeur = (og + od);
  else if (this->m_operateur == "-") valeur = (og - od);
  else if (this->m_operateur == "*") valeur = (og * od);
  else if (this->m_operateur == "==") valeur = (og == od);
  else if (this->m_operateur == "!=") valeur = (og != od);
  else if (this->m_operateur == "<") valeur = (og < od);
  else if (this->m_operateur == ">") valeur = (og > od);
  else if (this->m_operateur == "<=") valeur = (og <= od);
  else if (this->m_operateur == ">=") valeur = (og >= od);
  else if (this->m_operateur == "et") valeur = (og && od);
  else if (this->m_operateur == "ou") valeur = (og || od);
  else if (this->m_operateur == "non") valeur = (!og);
  else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
  }
  return valeur; // On retourne la valeur calculée
}

void NoeudOperateurBinaire::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    if (m_operandeGauche != nullptr){
        cout << "(";
        m_operandeGauche->traduitEnCPP(cout, indentation);
    }
      if (this->m_operateur == "+")cout << "+";
  else if (this->m_operateur == "-") cout << "-";
  else if (this->m_operateur == "*") cout << "*";
  else if (this->m_operateur == "==") cout << " == ";
  else if (this->m_operateur == "!=") cout << " != ";
  else if (this->m_operateur == "<") cout << " < ";
  else if (this->m_operateur == ">") cout << " > ";
  else if (this->m_operateur == "<=") cout << " <= ";
  else if (this->m_operateur == ">=") cout << " >= ";
  else if (this->m_operateur == "et") cout << " && ";
  else if (this->m_operateur == "ou") cout << " || ";
  else if (this->m_operateur == "non") cout << " ! ";
  else if (this->m_operateur == "/") cout << "/";
    if (m_operandeDroit != nullptr){
        m_operandeDroit->traduitEnCPP(cout, indentation);
        cout << ")";
    }
}

//////////////////////////////////////////////////////////////////////////////////
//// NoeudInstSi
//////////////////////////////////////////////////////////////////////////////////
//
//NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence)
//: m_condition(condition), m_sequence(sequence) {
//}
//
//int NoeudInstSi::executer() {
//  if (m_condition->executer()) m_sequence->executer();
//  return 0; // La valeur renvoyée ne représente rien !
//}





////////////////////////////////////////////////////////////////////////////////
// NoeudInstTantQue
////////////////////////////////////////////////////////////////////////////////
NoeudInstTantQue::NoeudInstTantQue(Noeud* expression, Noeud* sequence) : m_expression(expression), m_sequence(sequence){
}

int NoeudInstTantQue::executer(){
    while(m_expression->executer()){
        m_sequence->executer();
    }
    return 0;
}

void NoeudInstTantQue::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    cout << setw(indentation) << "" << "while(";
    m_expression->traduitEnCPP(cout,0);
    cout << "){" << endl;
    m_sequence->traduitEnCPP(cout, indentation+1);
    cout << setw(indentation) << "" << "}" << endl;
}






////////////////////////////////////////////////////////////////////////////////
// NoeudInstRepeter
////////////////////////////////////////////////////////////////////////////////
NoeudInstRepeter::NoeudInstRepeter(Noeud* sequence, Noeud* condSortie) : m_condSortie(condSortie), m_sequence(sequence){
}

int NoeudInstRepeter::executer(){
    do{
      m_sequence->executer();
    }
    while(!m_condSortie->executer());
        
    return 0;
}

void NoeudInstRepeter::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    cout << setw(indentation) << "" << "do{" << endl;
    m_sequence->traduitEnCPP(cout, indentation+1);
    cout << setw(indentation) << "" << "}" << endl;
    cout << setw(indentation) << "" << "while(!(";
    m_condSortie->traduitEnCPP(cout,0);
    cout << "))";
}





////////////////////////////////////////////////////////////////////////////////
// NoeudInstSiRiche
////////////////////////////////////////////////////////////////////////////////
NoeudInstSiRiche::NoeudInstSiRiche(){    
}

int NoeudInstSiRiche::executer(){
    bool siEffectue=false;
    for (int i=0; i<m_vectSiRiche.size(); i+=2) {
        if (m_vectSiRiche[i]->executer() && !siEffectue){
           m_vectSiRiche[i+1]->executer();
           siEffectue = true;
        }
    }
    return 0;
}

void NoeudInstSiRiche::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    cout << setw(indentation) << "" << "if (";
    m_vectSiRiche[0]->traduitEnCPP(cout,0);
    cout << "){" << endl;
    m_vectSiRiche[1]->traduitEnCPP(cout,indentation+1);
    cout << setw(indentation) << "" << "}" << endl;
    for (int i=2; i < m_vectSiRiche.size()-2; i+=2){
        cout << setw(indentation) << "" << "else if(";
        m_vectSiRiche[i]->traduitEnCPP(cout,0);
        cout << "){" << endl;
        m_vectSiRiche[i+1]->traduitEnCPP(cout,indentation+1);
        cout << setw(indentation) << "" << "}" << endl;
    }
    cout << setw(indentation) << "" << "else {" << endl;
    m_vectSiRiche[m_vectSiRiche.size()-1]->traduitEnCPP(cout,indentation+1);
    cout << setw(indentation) << "" << "}" << endl;
    
}


void NoeudInstSiRiche::ajoute(Noeud* instruction){
    if (instruction!=nullptr) m_vectSiRiche.push_back(instruction);
}




////////////////////////////////////////////////////////////////////////////////
// NoeudInstPour
////////////////////////////////////////////////////////////////////////////////
int NoeudInstPour::executer(){
//    (m_vecteurPour[0]!=nullptr)?: m_vecteurPour[0]->executer();
//    while(m_vecteurPour[1]->executer()){
//        m_vecteurPour[3]->executer();
//        (m_vecteurPour[2] != nullptr)?:m_vecteurPour[2]->executer();
//    }
    for((m_affectation1==nullptr)?: m_affectation1->executer(); m_expression1->executer(); (m_affectation2 == nullptr)?:m_affectation2->executer()){
        m_seq->executer();
    }
    return 0;
}

void NoeudInstPour::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    cout << setw(indentation) << "" << "for(";
    if(m_affectation1 != nullptr){
        m_affectation1->traduitEnCPP(cout,0);
        cout<<";";
    }
    m_expression1->traduitEnCPP(cout,0);
    if(m_affectation2 != nullptr){
        cout<< ";";
        m_affectation2->traduitEnCPP(cout,0);
    }
    cout << "){" << endl;
    m_seq->traduitEnCPP(cout,indentation+1);
    cout << setw(indentation) << "" << "}";
}



////////////////////////////////////////////////////////////////////////////////
// NoeudInstEcrire
////////////////////////////////////////////////////////////////////////////////
int NoeudInstEcrire::executer(){
    for(Noeud* element : m_vecteurEcrire){
        if ( (typeid(*element)==typeid(SymboleValue) && *((SymboleValue*)element) == "<CHAINE>")){
            string s = ((SymboleValue*)element)->getChaine();
            s.pop_back();
            s.erase(0,1);            
            cout << s;
        }
        else{
            cout<<element->executer();
        }
    }
    
    return 0;
}

void NoeudInstEcrire::ajoute(Noeud* instruction){
    if (instruction!=nullptr) m_vecteurEcrire.push_back(instruction);
}

void NoeudInstEcrire::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    cout << setw(indentation)<< "" << "cout ";
    for(Noeud* element : m_vecteurEcrire){
        cout << "<< ";
        if (typeid(*element)==typeid(SymboleValue) && *((SymboleValue*)element)== "<CHAINE>" ){
            cout << ((SymboleValue*)element)->getChaine();
        }
        element->traduitEnCPP(cout,0);
    }
}


////////////////////////////////////////////////////////////////////////////////
// NoeudInstLire
////////////////////////////////////////////////////////////////////////////////
int NoeudInstLire::executer(){
    int valeur;
    for(Noeud* element : m_vecteurLire){
        cin>>valeur;
        ((SymboleValue*)element)->getChaine();
        ((SymboleValue*)element)->setValeur(valeur);
    }
    return 0;

}

void NoeudInstLire::ajoute(Noeud* instruction){
    if (instruction!=nullptr) m_vecteurLire.push_back(instruction);
}

void NoeudInstLire::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    cout << setw(indentation) << "" << "cin ";
    for(Noeud* element : m_vecteurLire){
        cout << ">> ";
        element->traduitEnCPP(cout,0);
    }
}

NoeudInstSelon::NoeudInstSelon(Noeud* variable) : m_variable(variable) {
    
}

void NoeudInstSelon::ajoute(Noeud* instruction){
    if (instruction!=nullptr) m_vecteurSelon.push_back(instruction);
}

int NoeudInstSelon::executer(){
    switch(m_variable->executer()){
        for(static int i = 0; i < m_vecteurSelon.size(); i += 2){
            static const int a = m_vecteurSelon[i]->executer();
            case (a):
                m_vecteurSelon[i+1]->executer();
                break;            
        }
    }
    return 0;
}

void NoeudInstSelon::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    cout << setw(indentation) << "" << "switch(" << m_variable << "){" << endl;
    for(int i = 0; i < m_vecteurSelon.size(); i += 2){
        cout << setw(indentation+1) << "" << "case(" << m_vecteurSelon[i] << "):" << endl;
        m_vecteurSelon[i+1]->traduitEnCPP(cout, indentation+2);
        cout << setw(indentation+2) << "" << "break;" << endl;
    }
    cout << setw(indentation+1) << "" << "}" << endl;
}