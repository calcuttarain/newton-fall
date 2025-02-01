### ⭐ Monkey Testing - Newton's Fall (Raport de Testare)

**Scop:** Acest raport documentează testele de tip **Monkey Testing** efectuate pe jocul *Newton's Fall* pentru a identifica comportamente neașteptate și bug-uri.

---

## **1. Setup inițial**

Înainte de începerea testării, s-au realizat următorii pași pentru instalarea și configurarea jocului.

### **Instalare și rulare joc:**
```bash
git clone https://github.com/unibuc-cs/software-engineering-product-apples.git
cd software-engineering-product-apples
python3 -m venv .venv
source .venv/bin/activate
pip3 install -r requirements.txt
conan profile detect --force
conan install . --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
conan build .
./build/Release/bin/newtons_fall
```

### **Verificare rulare backend:**
```bash
cd software-engineering-product-apples/backend
npm install
./refresh.sh
npm run start
```

### **Verificare interfața CLI:**
```bash
cd software-engineering-product-apples/client
go run main.go
```

---

## **2. Obiectivul Monkey Test**

Monkey Testing a fost realizat fără un scenariu de testare prestabilit, iar testele au avut ca scop identificarea unor probleme precum:

- **Coliziuni anormale ale cubului**
- **Modul în care cubul pierdea viață**
- **Stabilitatea jocului în diverse scenarii**
- **Interacțiunile cu medii și obiecte aleatorii**
- **Comportamentul UI și răspunsul comenzilor**

---

## **3. Teste efectuate pe Newton's Fall**

### **3.1. Testarea mișcării cubului**

**Rezultate:**
- Cubul a fost mișcat în toate direcțiile rapid și aleatoriu.
- S-a încercat blocarea cubului între obstacole.
- Cubul a fost împins într-un colț pentru a se verifica dacă rămâne blocat.
- S-au apăsat simultan taste contradictorii (ex. stânga + dreapta).

**Observații:** Cubul a răspuns corect la comenzile de mișcare, dar s-a blocat în anumite situații.

---

### **3.2. Testarea coliziunilor cu pereții**

**Rezultate:**
- Cubul a fost împins în pereți din diverse unghiuri.
- S-a verificat dacă cubul putea trece prin pereți.
- S-a testat blocarea cubului între doi pereți strâmți.

**Observații:** Cubul nu a putut trece prin pereți, dar s-a blocat în unele coliziuni.

---

### **3.3. Testarea pierderii de viață**

**Rezultate:**
- Cubul a fost lovit repetat de pereți pentru a observa dacă pierdea viață corect.
- Jocul nu a resetat corect cubul la 0 HP.

**Observații:** La 0 HP, cubul a rămas într-o stare necontrolabilă.

---

### **3.4. Testarea stabilității și performanței**

**Rezultate:**
- Jocul a fost rulat timp de 30 de minute pentru a observa scurgeri de memorie.
- Alternarea rapidă între ferestre a cauzat lag.

**Observații:** Jocul a rămas stabil, dar schimbările bruște de fereastră au cauzat probleme.

---

### **3.5. Testarea bazei de date**

**Rezultate:**
- S-a modificat fișierul `.env` pentru a introduce o bază de date invalidă.
- Jocul nu a afișat mesaje clare de eroare la deconectarea bazei de date.

**Observații:** Backend-ul a gestionat erorile, dar interfața jocului nu a oferit mesaje explicite.

---

### **3.6. Testarea UI și input-ului**

**Rezultate:**
- S-au introdus caractere speciale în CLI pentru a testa gestionarea input-ului.
- Comenzile greșite nu au generat mesaje de eroare relevante.

**Observații:** UI-ul nu a gestionat bine input-ul invalid.

---

## **4. Concluzii și recomandări**

În urma testelor Monkey Testing, s-au identificat următoarele probleme:

❌ **Cubul s-a blocat în anumite coliziuni și nu a mai răspuns la input.**
❌ **La 0 HP, cubul nu s-a resetat corect.**
❌ **Interfața jocului nu a gestionat bine lipsa conexiunii la baza de date.**
❌ **Mesajele de eroare din CLI au fost insuficient explicite.**

**Recomandări:**
- Implementarea unei metode de deblocare a cubului în caz de coliziune prelungită.
- Asigurarea resetării corecte a cubului la 0 HP.
- Îmbunătățirea gestionării erorilor de conexiune cu backend-ul.
- Clarificarea mesajelor de eroare în interfața UI și CLI.

**Monkey Testing a evidențiat diverse probleme care pot fi remediate pentru a îmbunătăți experiența de joc!** 🚀🎮
