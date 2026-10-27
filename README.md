# Robus Combattant

Dépôt officiel du robot autonome pour la compétition **"Le rectangle"**. Le robot doit accomplir un parcours de suivi de ligne, détecter des couleurs, manipuler des objets (verre, quille) et franchir des obstacles en moins de 2 minutes.

---

## 📂 Structure du Projet

```text
projet_robot/
├── platformio.ini                   # Configuration PlatformIO (Carte Arduino Mega / Robot)
├── include/                         # Fichiers d'en-tête globaux
├── lib/                             # Bibliothèques externes (ex: LibRobus)
├── test/                            # Tests unitaires
└── src/
    ├── main.cpp                     # Point d'entrée principal (Naguy - Pilotage global)
    │
    ├── suivi_ligne/                 # 👤 Personne 2 : Charles
    │   ├── suivi_ligne.h
    │   └── suivi_ligne.cpp
    │
    ├── couleur_del/                 # 👤 Personne 3 : François
    │   ├── couleur_del.h
    │   └── couleur_del.cpp
    │
    ├── mecanisme/                   # 👤 Personne 4 : Jérémy
    │   ├── mecanisme.h
    │   └── mecanisme.cpp
    │
    ├── infrarouge/                  # 👤 Personne 5 : Naguy (Réception IR & Bips)
    │   ├── ir_recepteur.h
    │   └── ir_recepteur.cpp
    │
    └── obstacles/                   # 👤 Personne 5 : Naguy (Mur & Trou)
        ├── obstacles.h
        └── obstacles.cpp
```

---

## 👥 Répartition des Tâches

* **Naguy (Point d'entrée & Coordination)** : `main.cpp`, dossier `infrarouge/` (réception IR, bips de confirmation) et dossier `obstacles/` (mur, trou et intégration finale).
* **Charles (Suivi de ligne)** : Dossier `suivi_ligne/` (lecture analogique OPB704 et asservissement).
* **François (Couleurs & DELs)** : Dossier `couleur_del/` (détection des couleurs au sol et gestion des DELs pendant 10s).
* **Jérémy (Mécanismes)** : Dossier `mecanisme/` (servomoteurs, bras, pince, verre et quille).

---

## 📐 Bonnes Pratiques de Code (.h et .cpp)

Pour que notre code reste propre, modulaire et facile à fusionner, nous devons respecter rigoureusement la séparation entre les fichiers d'en-tête (`.h`) et d'implémentation (`.cpp`).

### 1. Rôles des fichiers `.h` (Header)
Le fichier `.h` sert d'interface publique pour le module. Il doit contenir **uniquement** :
* **Les gardes d'inclusion** (`#ifndef`, `#define`, `#endif`) Cest deja fait pour tout les .h
* **Les constantes globales** du module (broches GPIO, seuils de capteurs, timeouts, etc.).
* **Uniquement les signatures (prototypes) des fonctions publiques** que les autres coéquipiers (ou le `main.cpp`) ont le droit d'appeler. Les fonctions utilitaires internes (fonctions "helpers" privées) **ne doivent pas** figurer dans le `.h`.

**Exemple (`suivi_ligne.h`) :**
```cpp
#ifndef SUIVI_LIGNE_H
#define SUIVI_LIGNE_H

#include <config.h>

// --- CONSTANTES DU MODULE ---
const uint8_t PIN_CAPTEUR_GAUCHE = A0;
const uint8_t PIN_CAPTEUR_CENTRE = A1;
const uint8_t PIN_CAPTEUR_DROITE = A2;
const int SEUIL_NOIR = 600;

// --- SIGNATURES PUBLIQUES ---
/**
 * @brief Initialise les broches et la configuration du suiveur de ligne.
 */
void initialiserSuiviLigne();

/**
 * @brief Exécute la logique d'asservissement pour suivre la ligne anti-horaire.
 */
void executerSuiviLigne();

#endif
```

### 2. Rôles des fichiers `.cpp` (Implémentation)
Le fichier `.cpp` contient le code source réel (la logique) des fonctions déclarées dans le `.h`.
* Il doit inclure son propre fichier `.h` (`#include "suivi_ligne.h"`).
* C'est ici que l'on écrit les fonctions d'aide (helpers) privées qui n'ont pas besoin d'être exposées au reste de l'équipe.

### 3. Documentation des Fonctions (Format Doxygen)
Chaque fonction publique doit être documentée au-dessus de sa signature dans le fichier `.h` à l'aide de commentaires au format Doxygen :

```cpp
/**
 * @brief Brève description de ce que fait la fonction.
 * 
 * Description plus détaillée si nécessaire (comportement, contraintes temporelles, etc.).
 * 
 * @param param1 Description du premier paramètre d'entrée.
 * @param param2 Description du deuxième paramètre.
 * @return int   Description de la valeur de retour (ex: 0 pour succès, -1 si erreur).
 */
int maFonctionExemple(int param1, float param2);
```

---

## 🚀 Instructions de Compilation (PlatformIO)

1. Ouvrir le dossier `projet_robot/` dans **VS Code**.
2. S'assurer d'avoir installé l'extension **PlatformIO**.
3. Compiler le projet en cliquant sur l'icône de coche (`✔ Build`) dans la barre inférieure.
4. Connecter la carte Arduino Mega du robot et téléverser le code via l'icône de flèche (`➡ Upload`).