# 🚀 Configuration minimale pour cloner et compiler un projet PlatformIO ESP32

Ce guide explique les étapes nécessaires pour récupérer un dépôt Git contenant un projet **PlatformIO** destiné à une carte **ESP32**, puis le compiler et le téléverser.

---

## 📋 Prérequis

Avant de commencer, assurez-vous d’avoir installé :

### **Extension PlatformIO IDE**
Depuis VS Code :  
`Extensions → Rechercher "PlatformIO IDE" → Installer`

---

## 🧰 Dépendances gérées automatiquement

Une fois le projet ouvert dans PlatformIO, les éléments suivants seront installés automatiquement :

- Toolchain ESP32  
- Framework (ESP-IDF ou Arduino selon le `platformio.ini`)
- Librairies déclarées dans le fichier `platformio.ini`

Aucune installation manuelle supplémentaire n’est normalement requise.

---

## 📦 Cloner le dépôt

```bash
git clone https://github.com/username/nom-du-projet.git
cd nom-du-projet
