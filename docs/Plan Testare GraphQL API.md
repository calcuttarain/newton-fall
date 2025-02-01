# Plan de Testare pentru API-ul GraphQL

## 1. Obiectivele Testării
Testarea are ca scop verificarea corectitudinii și funcționalității API-ului GraphQL, incluzând:
- **Autentificare și Autorizare**: verificarea procesului de autentificare și permisiunile utilizatorilor.
- **CRUD pentru Utilizatori**: crearea, autentificarea și ștergerea utilizatorilor.
- **Manipularea Scorurilor**: adăugarea și interogarea scorurilor utilizatorilor.
- **Erori și scenarii negative**: testarea cazurilor de eșec (ex. autentificare eșuată, acces neautorizat).

## 2. Procesul Testării
Testarea este realizată iterativ pe parcursul dezvoltării software, cu integrare în **CI/CD**:
- **În timpul dezvoltării**: rularea testelor unitare și de integrare folosind `node:test`.
- **Înainte de integrarea în branch-ul principal**: rularea testelor automatizate pe branch-ul `Testing` utilizând GitHub Actions.
- **Înainte de lansare**: testare manuală și validare a rezultatelor în medii de staging.

## 3. Metodele Testării

| Metodă | Descriere | Aplicare |
|--------|------------|------------|
| **Testare Unitara** | Verificarea corectitudinii funcțiilor individuale | Teste pentru autentificare, creare utilizator, creare scor |
| **Testare de Integrare** | Testarea interacțiunii între module (ex. API și baza de date) | Teste GraphQL care simulează utilizarea API-ului |
| **Testare de Regresie** | Verificarea că modificările nu afectează funcționalitatea existentă | Se rulează testele automat la fiecare commit |
| **Testare Funcțională** | Validarea cerințelor de business și comportamentul API-ului | Se verifică scenariile de utilizare |

## 4. Rezultatele Testării
Testele au fost implementate și rulate cu succes, obținând următoarele observații:

### Teste rulate
- **Creare utilizator:** ✅ Utilizatorul este creat cu succes.
- **Autentificare utilizator:** ✅ Se primește un token valid.
- **Creare scor:** ✅ Scorul este salvat corect în baza de date.
- **Interogare scoruri:** ✅ Datele sunt returnate corect pentru utilizatorul autentificat.
- **Eșec autentificare (parolă greșită):** ✅ Sistemul nu returnează un token.
- **Creare scor fără autentificare:** ✅ Sistemul returnează eroare de autorizare.
- **Ștergere utilizator:** ✅ Contul este eliminat cu succes.

### Concluzii
- Testele automate rulează fără erori atât local, cât și în pipeline-ul de **GitHub Actions**.
- Implementarea actuală gestionează corect cazurile de utilizare standard și scenariile de eroare.
- Posibile îmbunătățiri: adăugarea unor teste de **performanță** și **scalabilitate** pentru interogările GraphQL.

## 5. Integrarea în CI/CD
Testele sunt integrate în pipeline-ul de **GitHub Actions**, rulând pe **Ubuntu** și **macOS**, conform fișierului de configurare. La fiecare commit pe branch-ul `Testing`, testele sunt executate automat.

---
**Status Final:** ✅ Toate testele au fost rulate cu succes.

