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
  for (unsigned int i = 0; i < m_instructions.size(); i++)
    m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}


void NoeudSeqInst::traduitEnCPP(ostream& cout, unsigned int indentation) const{
    for (unsigned int i = 0; i < m_instructions.size(); i++){
        m_instructions[i]->traduitEnCPP(cout, indentation);
        
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
    cout << ";" << endl;
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
        m_operandeGauche->traduitEnCPP(cout, indentation);
    }
    cout << m_operateur;
    if (m_operandeDroit != nullptr){
        m_operandeDroit->traduitEnCPP(cout, indentation);
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
    cout << setw(indentation) << "while(";
    m_expression->traduitEnCPP(cout,0);
    cout << "){" << endl;
    m_sequence->traduitEnCPP(cout, indentation+1);
    cout << endl << "}" << endl;
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
    cout << setw(indentation) << "do{" << endl;
    m_sequence->traduitEnCPP(cout, indentation+1);
    cout << setw(indentation) << "}" << endl;
    cout << setw(indentation) << "while(";
    m_condSortie->traduitEnCPP(cout,0);
    cout << ");" << endl;
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
    cout << setw(4*indentation) << "" << "if (";
    for (int i=0; i < m_vectSiRiche.size()-2; i+=2){
        if(m_vectSiRiche[i] != nullptr){
            m_vectSiRiche[i]->traduitEnCPP(cout,0);
            cout << ")";    
        }
        cout<< "{" << endl;
        m_vectSiRiche[i+1]->traduitEnCPP(cout,1);
    }
    cout << setw(4*indentation) << "" << "}" << endl;
    
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
    cout << setw(indentation) << "for(";
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
    cout << ";" << endl;
    cout << setw(indentation) << "}";
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
            cout<<s;
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
    cout << setw(indentation) << "cout ";
    for(Noeud* element : m_vecteurEcrire){
        cout << "<< ";
        element->traduitEnCPP(cout,0);
    }
    cout << ");" << endl;
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
    cout << setw(indentation) << "cin ";
    for(Noeud* element : m_vecteurLire){
        cout << ">> ";
        element->traduitEnCPP(cout,0);
    }
    cout << ");" << endl;
}