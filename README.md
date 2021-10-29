# iChauffage
Code source du système de contrôle du chauffage domestique par la domotique.

## Matériel
- *Microcontrôleur* | ESP32 (≈ 10€)
- *Capteur de température et d'humidité* | DHT22 (≈ 5€)
- *Relais 5V* (≈ 5€)
- *Transistor bipolaire NPN* | 2N2222 ou similaire (< 1€)
- *Quelques résistances* (< 1€)
- *Boîtier, alimentation 5V et câbles* | Récupération

**Total: ≈ 22€**

## Fonctionnement
La chaudière est équipée d'un thermostat mécanique à placer dans l'habitat. Le thermostat fait contact lorsque la température ambiante est inférieure à la température réglée et coupe le contact quand il fait suffisamment chaud.  
Le projet se place en dérivation avec le thermostat pour continuer à utiliser l'ancien système si jamais il tombe en panne.
