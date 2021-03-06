#ifndef ARBREABSTRAIT_H
#define ARBREABSTRAIT_H

// Contient toutes les déclarations de classes nécessaires
//  pour représenter l'arbre abstrait

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
class Noeud {
// Classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
// Remarque : la classe ne contient aucun constructeur
  public:
    virtual int  executer() =0 ; // Méthode pure (non implémentée) qui rend la classe abstraite
    virtual void ajoute(Noeud* instruction) { throw OperationInterditeException(); }
    virtual ~Noeud() {} // Présence d'un destructeur virtuel conseillée dans les classes abstraites
    virtual void traduitEnCPP(ostream & cout, unsigned int indentation) const{};
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud {
// Classe pour représenter un noeud "sequence d'instruction"
//  qui a autant de fils que d'instructions dans la séquence
  public:
     NoeudSeqInst();   // Construit une séquence d'instruction vide
    ~NoeudSeqInst() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();    // Exécute chaque instruction de la séquence
    void ajoute(Noeud* instruction);  // Ajoute une instruction à la séquence
    void traduitEnCPP(ostream & cout, unsigned int indentation)const;
        
  private:
    vector<Noeud *> m_instructions; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud {
// Classe pour représenter un noeud "affectation"
//  composé de 2 fils : la variable et l'expression qu'on lui affecte
  public:
     NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
    ~NoeudAffectation() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();        // Exécute (évalue) l'expression et affecte sa valeur à la variable
    void traduitEnCPP(ostream & cout, unsigned int indentation)const;

  private:
    Noeud* m_variable;
    Noeud* m_expression;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud {
// Classe pour représenter un noeud "opération binaire" composé d'un opérateur
//  et de 2 fils : l'opérande gauche et l'opérande droit
  public:
    NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
    // Construit une opération binaire : operandeGauche operateur OperandeDroit
   ~NoeudOperateurBinaire() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();            // Exécute (évalue) l'opération binaire)
    void traduitEnCPP(ostream & cout, unsigned int indentation) const;
    
  private:
    Symbole m_operateur;
    Noeud*  m_operandeGauche;
    Noeud*  m_operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////
//class NoeudInstSi : public Noeud {
//// Classe pour représenter un noeud "instruction si"
////  et ses 2 fils : la condition du si et la séquence d'instruction associée
//  public:
//    NoeudInstSi(Noeud* condition, Noeud* sequence);
//     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
//   ~NoeudInstSi() {} // A cause du destructeur virtuel de la classe Noeud
//    int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence
//
//  private:
//    Noeud*  m_condition;
//    Noeud*  m_sequence;
//};

class NoeudInstTantQue : public Noeud{
//Classe pour représenter un noeud "tant que"
    
    public:
         NoeudInstTantQue(Noeud* expression, Noeud* sequence); //Construit un "tant que" avec son expression et sa séquence d'instruction
         ~NoeudInstTantQue(){}; // A cause du destructeur virtuel de la classe Noeud
        int executer(); // Exécute l'instruction si : si condition vraie on exécute la séquence
        void traduitEnCPP(ostream & cout, unsigned int indentation) const;
    
    private:
        Noeud* m_expression;
        Noeud* m_sequence;
};

  
class NoeudInstSiRiche : public Noeud {
    //Classe pour représenter un noeud "instruction si riche"
    // et ses 5 fils : la condition du si initial, la seq d'inst inital, un vecteur de condition sinonsi,
    // un vecteur de seq d'inst sinonsi, une seq d'int sinon
    public :
        NoeudInstSiRiche();
       ~NoeudInstSiRiche() {}// A cause du destructeur virtuel de la classe Noeud
       int executer();
       void ajoute(Noeud* instruction);// Ajoute une instruction à la séquence
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
    private:
        
        vector<Noeud*> m_vectSiRiche;
};


class NoeudInstRepeter : public Noeud {
    //Classe pour représenter un noeud "instruction si riche"
    // et ses 2 fils: sequence a repeter et condition de sortie de boucle
    public:
        NoeudInstRepeter(Noeud* sequence, Noeud* condSortie);
       ~NoeudInstRepeter() {};// A cause du destructeur virtuel de la classe Noeud
        int executer();
        void traduitEnCPP(ostream & cout, unsigned int indentation) const;
       
    private:
        Noeud* m_sequence;
        Noeud* m_condSortie;
};

class NoeudInstPour : public Noeud{
    //Classe pour représenter un noeud "instruction pour"
    // et ses fils: affectation, expression, affectation et seqInst
    public:
        NoeudInstPour(Noeud* affectation1, Noeud* expression1, Noeud* affectation2, Noeud* seq): m_affectation1(affectation1), m_expression1(expression1), m_affectation2(affectation2), m_seq(seq){};
       ~NoeudInstPour(){};// A cause du destructeur virtuel de la classe Noeud
        int executer();
        void traduitEnCPP(ostream & cout, unsigned int indentation) const;
       
    private:
        Noeud* m_affectation1;
        Noeud* m_expression1;
        Noeud* m_affectation2;
        Noeud* m_seq;
};

class NoeudInstEcrire : public Noeud{
    //Classe pour représenter un noeud "instruction écrire"
    
    public:
        NoeudInstEcrire(){};
       ~NoeudInstEcrire(){};// A cause du destructeur virtuel de la classe Noeud
       void ajoute(Noeud* noeud);// Ajoute une instruction à la séquence
       int executer();
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
       
    private:
        vector<Noeud*> m_vecteurEcrire;

    
    
};

class NoeudInstLire : public Noeud{
    //Classe pour représenter un noeaud "instruction écrire"
    public:
        NoeudInstLire(){};
       ~NoeudInstLire(){};// A cause du destructeur virtuel de la classe Noeud
       void ajoute(Noeud* noeud);// Ajoute une instruction à la séquence
       int executer();
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
        
    private:
        vector<Noeud*> m_vecteurLire;    
    
};

class NoeudIncr : public Noeud{
    //Classe pour représenter un noeaud "instruction écrire"
    public:
        NoeudIncr(Noeud* variable);
       ~NoeudIncr(){};// A cause du destructeur virtuel de la classe Noeud
       int executer();
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
        
    private:
        Noeud* m_variable;    
    
};

class NoeudDecr : public Noeud{
    //Classe pour représenter un noeaud "instruction écrire"
    public:
        NoeudDecr(Noeud* variable);
       ~NoeudDecr(){};// A cause du destructeur virtuel de la classe Noeud
       int executer();
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
        
    private:
        Noeud* m_variable;    
    
};


        
class NoeudAffectationDirecteSoustraction : public Noeud{
    //Classe pour représenter un noeaud "instruction écrire"
    public:
        NoeudAffectationDirecteSoustraction(Noeud* variable, Noeud* operande);
       ~NoeudAffectationDirecteSoustraction(){};// A cause du destructeur virtuel de la classe Noeud
       int executer();
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
        
    private:
        Noeud* m_variable;    
        Noeud* m_operande;
    
};




class NoeudAffectationDirecteAddition : public Noeud{
    //Classe pour représenter un noeaud "instruction écrire"
    public:
        NoeudAffectationDirecteAddition(Noeud* variable, Noeud* operande);
       ~NoeudAffectationDirecteAddition(){};// A cause du destructeur virtuel de la classe Noeud
       int executer();
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
        
    private:
        Noeud* m_variable;    
        Noeud* m_operande;
    
};


class NoeudAffectationDirecteMultiplication : public Noeud{
    //Classe pour représenter un noeaud "instruction écrire"
    public:
        NoeudAffectationDirecteMultiplication(Noeud* variable, Noeud* operande);
       ~NoeudAffectationDirecteMultiplication(){};// A cause du destructeur virtuel de la classe Noeud
       int executer();
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
        
    private:
        Noeud* m_variable;    
        Noeud* m_operande;
    
};  

class NoeudAffectationDirecteDivision : public Noeud{
    //Classe pour représenter un noeaud "instruction écrire"
    public:
        NoeudAffectationDirecteDivision(Noeud* variable, Noeud* operande);
       ~NoeudAffectationDirecteDivision(){};// A cause du destructeur virtuel de la classe Noeud
       int executer();
       void traduitEnCPP(ostream & cout, unsigned int indentation) const;
        
    private:
        Noeud* m_variable;    
        Noeud* m_operande;
    
}; 
//class NoeudInstSelon : public Noeud{
//    //Classe pour représenter un noeud "instruction selon"
//    public:
//        NoeudInstSelon(Noeud* variable);
//        ~NoeudInstSelon(){};
//        void ajoute(Noeud* noeud);
//        int executer();
//        void traduitEnCPP(ostream & cout, unsigned int indentation) const;
//        
//    private:
//        vector<Noeud*> m_vecteurSelon;
//        Noeud* m_variable;
//};    



#endif
