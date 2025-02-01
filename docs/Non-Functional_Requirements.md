### ⭐ Non-Functional Requirements pentru Newton’s Fall

Aceste cerințe definesc modul în care jocul *Newton’s Fall* funcționează din punct de vedere al performanței, securității, disponibilității și altor aspecte tehnice.

---

## **1. Performanță și Scalabilitate**
✅ **Timp de răspuns:**  
   - Jocul menține un **frame rate minim de 60 FPS** pe hardware-ul suportat.  
   - Timpul de încărcare al nivelurilor nu depășește **5 secunde**.  

✅ **Optimizare grafică:**  
   - Texturile și shader-ele sunt optimizate pentru a minimiza utilizarea resurselor GPU.  
   - Coliziunile și fizica sunt procesate eficient pentru a evita frame drops.  

✅ **Scalabilitate:**  
   - Jocul rulează atât pe sisteme high-end, cât și pe configurații mai slabe, prin suportul setărilor grafice ajustabile.  

---

## **2. Securitate**
✅ **Protecție anti-cheat:**  
   - Sistemul detectează și previne modificarea fișierelor jocului (ex. valori ale vieții, fizică, coliziuni).  

✅ **Protecția datelor utilizatorului:**  
   - Dacă există un sistem de autentificare sau stocare de date online, acestea sunt criptate și protejate de atacuri SQL Injection sau XSS.  

---

## **3. Disponibilitate și Fiabilitate**
✅ **Stabilitate:**  
   - Jocul nu se blochează în timpul rulării, iar gestionarea erorilor previne crash-uri critice.  

✅ **Recuperare automată:**  
   - Dacă jocul detectează o eroare gravă, acesta revine la un ecran de siguranță fără a închide aplicația.  

✅ **Toleranță la erori:**  
   - Dacă backend-ul nu este disponibil, jocul continuă să funcționeze fără întreruperi majore.  

---

## **4. Ușurința în utilizare (Usability)**
✅ **Accesibilitate:**  
   - Interfața este intuitivă, iar comenzile sunt configurabile.  
   - Jocul oferă un mod de control alternativ pentru jucători cu dizabilități.  

✅ **Feedback vizual și audio:**  
   - Jucătorul primește **feedback clar** la acțiunile efectuate (ex. când pierde viață, când lovește un perete etc.).  

✅ **Timp minim de învățare:**  
   - Mecanica jocului este explicată clar într-un tutorial sau într-un ecran dedicat.  

---

## **5. Portabilitate și Compatibilitate**
✅ **Compatibilitate cross-platform:**  
   - Jocul este compatibil cu Windows, Linux și macOS.  
   - Acesta rulează atât pe configurații cu NVIDIA, AMD, cât și Intel Graphics.  

✅ **Suport pentru diverse periferice:**  
   - Jocul funcționează cu tastatură + mouse și permite suport opțional pentru controlere.  

✅ **Compatibilitate cu backend-ul:**  
   - Dacă există o componentă online, aceasta este compatibilă cu diverse versiuni de backend.  

---

## **6. Mentenabilitate și Extensibilitate**
✅ **Cod modular:**  
   - Codul sursă este organizat clar pentru a facilita modificările și îmbunătățirile viitoare.  
   - Fizica, randarea grafică și logica jocului sunt separate în module independente.  

✅ **Documentație actualizată:**  
   - Documentația pentru cod, API și configurarea jocului este disponibilă și actualizată.  

✅ **Facilitatea de debugging:**  
   - Jocul conține **log-uri de eroare** și un mod de debugging activabil.  

---

## **7. Testabilitate**
✅ **Testare automată:**  
   - Componentele critice ale jocului (fizica, coliziunile, interacțiunile) sunt testate automat pentru a detecta regresii.  

✅ **Testare Monkey:**  
   - Jocul gestionează corect input aleatoriu fără să crasheze (vezi raportul de Monkey Testing).  

✅ **Testare de performanță:**  
   - Benchmark-urile pentru consumul de CPU/GPU și utilizarea memoriei sunt realizate pentru a asigura optimizarea jocului.  

---

## **📌 Concluzie**
Aceste **Non-Functional Requirements** asigură o experiență de joc **fluidă, sigură și optimizată** pentru diverse platforme. Chiar dacă mecanicile jocului sunt corect implementate, problemele de performanță, securitate sau utilizabilitate pot afecta negativ experiența jucătorilor. 🚀🎮
