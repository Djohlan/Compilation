#include <stdio.h>
#include <stdlib.h>
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "premiers.h"
#include "suivants.h"
#include "symboles.h"
#include "util.h"


extern char yytext[100];
FILE *yyin;
int uniteCourante;
extern int nb_ligne;
int trace_xml=1;


/*********************************************************************
 * GRAMMAIRE
 ********************************************************************/

void PG()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_optDecVariables_, uniteCourante) ||
		est_premier(_listeDecFonctions_, uniteCourante) ||
		est_suivant(_programme_, uniteCourante) ) {	// 1
		ODV();
		LDF();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void ODV()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_listeDecVariables_, uniteCourante) ) {	// 2
		LDV();
		if(uniteCourante == POINT_VIRGULE) {
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	// EPSILON
	if( est_suivant(_optDecVariables_, uniteCourante) ) {	// 3
		/* rien */ 
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void LDV()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_declarationVariable_, uniteCourante) ) {	// 4
		DV();
		LDVB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void LDVB()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == VIRGULE) {	// 5
		affiche_element ("symbole", yytext, trace_xml);
		uniteCourante = yylex();
		DV();
		LDVB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_listeDecVariablesBis_, uniteCourante) ) {	// 6
		 /* rien */ 
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void DV()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	printf ("%s: yytext = %s\n", __func__, yytext);
	// TERMINAL
	if(uniteCourante == ENTIER) {	// 7
		affiche_element ("mot_clef", yytext, trace_xml);
		uniteCourante = yylex();
		if(uniteCourante == ID_VAR) {
			affiche_element ("id_variable", yytext, trace_xml);
			printf ("%s: yytext = %s\n", __func__, yytext);
			uniteCourante = yylex();
			printf ("%s: yytext = %s\n", __func__, yytext);
			OTT();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void OTT()
{
	printf ("%s: coucou\n", __func__);
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == CROCHET_OUVRANT) {	// 8
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		if(uniteCourante == NOMBRE) {
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			if(uniteCourante == CROCHET_FERMANT) {
				affiche_element (__FUNCTION__, yytext, trace_xml);
				uniteCourante = yylex();
				affiche_balise_fermante(__FUNCTION__, trace_xml);
				return;
			}
			erreur_1s ("Erreur de syntaxe\n", yytext);
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	// EPSILON
	if( est_suivant(_optTailleTableau_, uniteCourante) ) {	// 9
		 /* rien */ 
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void LDF()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_declarationFonction_, uniteCourante) ) {	// 10
		DF();
		LDF();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_listeDecFonctions_, uniteCourante) ) {	// 11
		 /* rien */ 
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}
 
void DF()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == ID_FCT) {	// 12
		affiche_element ("id_fonction", yytext, trace_xml);
		uniteCourante = yylex();
		LP();
		ODV();
		IB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}
 
void LP()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == PARENTHESE_OUVRANTE) {	// 13
		affiche_element ("symbole", yytext, trace_xml);
		uniteCourante = yylex();
		OLDV();
		if(uniteCourante == PARENTHESE_FERMANTE) {
			affiche_element ("symbole", yytext, trace_xml);
			uniteCourante = yylex();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void OLDV()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_listeDecVariables_, uniteCourante) ) {	// 14
		LDV();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_optListeDecVariables_, uniteCourante) ) {	// 15
		 /* rien */ 
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void I()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_instructionAffect_, uniteCourante) ) {	// 16
		IAFF();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if( est_premier(_instructionBloc_, uniteCourante) ) {	// 17
		IB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if( est_premier(_instructionSi_, uniteCourante) ) {	// 18
		ISI();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if( est_premier(_instructionTantque_, uniteCourante) ) {	// 19
		ITQ();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if( est_premier(_instructionAppel_, uniteCourante) ) {	// 20
		IAPP();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if( est_premier(_instructionRetour_, uniteCourante) ) {	// 21
		IRET();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if( est_premier(_instructionEcriture_, uniteCourante) ) {	// 22
		IECR();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if( est_premier(_instructionVide_, uniteCourante) ) {	// 23
		IVIDE();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void IAFF()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_var_, uniteCourante) ) {	// 24
		VAR();
		if(uniteCourante == EGAL) {	// 
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			EXP();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void IB()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == ACCOLADE_OUVRANTE) {	// 25
		affiche_element ("symbole", yytext, trace_xml);
		uniteCourante = yylex();
		LI();
		if(uniteCourante == ACCOLADE_FERMANTE) {
			affiche_element ("symbole", yytext, trace_xml);
			uniteCourante = yylex();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void LI()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_instruction_, uniteCourante) ) {	// 26
		I();
		LI();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_listeInstructions_, uniteCourante) ) {	// 27
		 /* rien */ 
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void ISI()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == SI) {	// 28
		affiche_element ("mot-clef", yytext, trace_xml);
		uniteCourante = yylex();
		EXP();
		if(uniteCourante == ALORS) {
			affiche_element ("mot-clef", yytext, trace_xml);
			uniteCourante = yylex();
			IB();
			OSINON();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void OSINON()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == SINON) {	// 29
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		IB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_optSinon_, uniteCourante) ) {	// 30
		 /* rien */ 
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void ITQ()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == TANTQUE) {	// 31
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		EXP();
		if(uniteCourante == FAIRE) {
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			IB();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void IAPP()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_appelFct_, uniteCourante) ) {	// 32
		APPF();
		if(uniteCourante == POINT_VIRGULE) {
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void IRET()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == RETOUR) {	// 33
		affiche_element ("mot-clef", yytext, trace_xml);
		uniteCourante = yylex();
		EXP();
		if(uniteCourante == POINT_VIRGULE) {
			affiche_element ("symbole", yytext, trace_xml);
			uniteCourante = yylex();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void IECR()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == ECRIRE) {	// 34
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			EXP();
			if(uniteCourante == PARENTHESE_FERMANTE) {
				affiche_element (__FUNCTION__, yytext, trace_xml);
				uniteCourante = yylex();
				if(uniteCourante == POINT_VIRGULE) {
					affiche_element (__FUNCTION__, yytext, trace_xml);
					uniteCourante = yylex();
					affiche_balise_fermante(__FUNCTION__, trace_xml);
					return;
				}
				erreur_1s ("Erreur de syntaxe\n", yytext);
			}
			erreur_1s ("Erreur de syntaxe\n", yytext);
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void IVIDE()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == POINT_VIRGULE) {	// 35
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

///////////////////////////////////////////////////////////////////////////////

void EXP() 
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_conjonction_, uniteCourante) ) {	// 36
		CONJ();
		EXPB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void EXPB()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == OU) {	// 37
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		CONJ();
		EXPB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_expressionBis_, uniteCourante) ) {	// 38
		/* rien */
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void CONJ()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_comparaison_, uniteCourante) ) {	// 39
		COMP();
		CONJB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void CONJB()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == ET) {	// 40
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		COMP();
		CONJB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_conjonctionBis_, uniteCourante) ) {	// 41
		/* rien */
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void COMP()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_expArith_, uniteCourante) ) {	// 42
		E();
		COMPB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void COMPB()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == EGAL ||   		// R 43
	   uniteCourante == INFERIEUR) {  	// R 44
		affiche_element ("symbole", yytext, trace_xml);
		uniteCourante = yylex();
		E();
		COMPB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_comparaisonBis_, uniteCourante) ) {	// 45
		/* rien */
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void E() 
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_terme_, uniteCourante) ) {	// 46
		T();
		EB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void EB()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == PLUS ||   		// règle 47
	   uniteCourante == MOINS) {  		//       48
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		T();
		EB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_expArithBis_, uniteCourante) ) {
		/* rien */
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void T()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// NON TERMINAL
	if( est_premier(_negation_, uniteCourante) ) {
		NEG();
		TB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void TB()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == FOIS ||   		// règle 51
	   uniteCourante == DIVISE) {  		//       52
		affiche_element ("TB", yytext, trace_xml);
		uniteCourante = yylex();
		NEG();
		TB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_termeBis_, uniteCourante) ) {
		/* rien */
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void NEG()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == NON) {
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		NEG();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// NON TERMINAL
	if( est_premier(_facteur_, uniteCourante) ) {
		F();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void F()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == PARENTHESE_OUVRANTE) {	// 56
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		EXP();
		if(uniteCourante == PARENTHESE_FERMANTE) {
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	if(uniteCourante == NOMBRE) {	// 57
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if(uniteCourante == LIRE) {	// 60
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			if(uniteCourante == PARENTHESE_FERMANTE) {
				affiche_element (__FUNCTION__, yytext, trace_xml);
				uniteCourante = yylex();
				affiche_balise_fermante(__FUNCTION__, trace_xml);
				return;
			}
			erreur_1s ("Erreur de syntaxe\n", yytext);
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	// NON TERMINAL
	if( est_premier(_appelFct_, uniteCourante) ) {	// 58
		APPF();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	if( est_premier(_var_, uniteCourante) ) {	// 59
		VAR();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void VAR()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == ID_VAR) {	// 61
		affiche_element ("id-variable", yytext, trace_xml);
		uniteCourante = yylex();
		OIND();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void OIND()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == CROCHET_OUVRANT) {	// 62
		affiche_element ("symbole", yytext, trace_xml);
		uniteCourante = yylex();
		EXP();
		if(uniteCourante == CROCHET_FERMANT) {
			affiche_element ("symbole", yytext, trace_xml);
			uniteCourante = yylex();
			affiche_balise_fermante(__FUNCTION__, trace_xml);
			return;
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	// EPSILON
	if( est_suivant(_optIndice_, uniteCourante) ) {	// 63
		/* rien */
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void APPF()
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == ID_FCT) {	// 64
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		if(uniteCourante == PARENTHESE_OUVRANTE) {
			affiche_element (__FUNCTION__, yytext, trace_xml);
			uniteCourante = yylex();
			LEXP();
			if(uniteCourante == PARENTHESE_FERMANTE) {
				affiche_element (__FUNCTION__, yytext, trace_xml);
				uniteCourante = yylex();
				affiche_balise_fermante(__FUNCTION__, trace_xml);
				return;
			}
			erreur_1s ("Erreur de syntaxe\n", yytext);
		}
		erreur_1s ("Erreur de syntaxe\n", yytext);
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void LEXP()
{
	// NON TERMINAL
	if( est_premier(_expression_, uniteCourante) ) {	// 65
		EXP();
		LEXPB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_listeExpressions_, uniteCourante) ) {	// 66
		/* rien */
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}

void LEXPB() 
{
	affiche_balise_ouvrante (__FUNCTION__, trace_xml);
	// TERMINAL
	if(uniteCourante == VIRGULE) {	// 67
		affiche_element (__FUNCTION__, yytext, trace_xml);
		uniteCourante = yylex();
		EXP();
		LEXPB();
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	// EPSILON
	if( est_suivant(_listeExpressionsBis_, uniteCourante) ) {	// 68
		/* rien */
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return;
	}
	erreur_1s ("Erreur de syntaxe\n", yytext);
}


/*********************************************************************
 * MAIN
 ********************************************************************/

int main(int argc, char **argv) 
{
  yyin = fopen(argv[1], "r");  // pour fichier
  if(yyin == NULL) {
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  
  initialise_premiers();
  initialise_suivants();
  
  uniteCourante = yylex();
  PG();
  uniteCourante = FIN;
  printf ("\n%s: FIN\n", __FUNCTION__);
  
  return 0;
}





