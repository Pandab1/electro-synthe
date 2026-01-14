# Interface UI – Synthétiseur

## 1. Navigation générale

### 1.1 Menu latéral (sections)

- **Oscillator**
- **Enveloppe sonore**
- **Output**
- **Help**

Ce menu latéral permet de naviguer rapidement entre les grandes zones de configuration du synthé.

---

### 1.2 Barre d’état (status bar)

Barre située en bas de l’écran affichant en temps réel :

- **État du moteur audio** : `ON` / `OFF`
- **Forme d’onde actuelle**
- **Fréquence** (en Hz)
- **Volume**
- **Indicateur de phase**

---

### 1.3 Zone d’aide dynamique

Zone dédiée aux raccourcis clavier et à l’aide contextuelle, mise à jour en fonction de la section active.

Exemples de raccourcis affichés :

- `ESPACE` : Play / Stop  
- `Q` : Quitter  
- `↑ / ↓` : Monter / baisser le volume  
- `← / →` : Changer de forme d’onde  
- (Autres raccourcis possibles selon les besoins…)

---

## 2. Widgets interactifs

### 2.1 Sélection de la forme d’onde

- **4 boutons de forme d’onde** :
  - Sinusoïdale
  - Carrée
  - Triangulaire
  - Dent de scie

Affichage possible sous forme de boutons alignés :

`[ Sin ]  [ Carrée ]  [ Triangulaire ]  [ Dent de scie ]`

La forme active peut être mise en évidence (encadrée, inversée, surlignée, etc.).

---

### 2.2 Contrôle de fréquence

- **1 “slicer” de fréquence** (curseur horizontal)
  - Contrôle par `+ / -` ou touches dédiées
  - Affichage de la valeur courante en Hz  
  - Exemple visuel en ASCII :

    ```
    Fréquence : 440 Hz
    [||||||||----]
    ```

---

### 2.3 Contrôle de volume

- **1 “slicer” de volume**
  - Même logique que la fréquence
  - Affichage en pourcentage ou en dB  

    ```
    Volume : 75 %
    [|||||||-----]
    ```

---

### 2.4 Bouton Play / Stop

- **1 bouton** : `Play / Stop`
  - Change d’apparence selon l’état :
    - `ON` : couleur / style **actif**
    - `OFF` : couleur / style **neutre**
  - La barre d’état reflète aussi cet état :  
    `Audio Engine : ON` ou `Audio Engine : OFF`

---

## 3. Enveloppe sonore (ADSR)

Ajout de texte et de champs de configuration pour l’enveloppe sonore :

- **Attack time**
- **Decay time**
- **Sustain**
- **Release**

Exemple de présentation :

```text
Enveloppe ADSR
--------------
Attack  : [   ] ms
Decay   : [   ] ms
Sustain : [   ] %
Release : [   ] ms
