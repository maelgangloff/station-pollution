# Projet Enseignement Scientifique

Station de mesure de la quantité en particules fines dans l'atmosphère (et de la concentration en monoxyde de carbone).  
Nous réalisons une étude comparative des concentrations en particules fines PM2.5 et PM10.  
Problématique: **Dans quelles mesures les activités humaines polluent l'atmosphère par des particules fines ?**  

## I. Ce que l'on sait sur les particules fines et la pollution atmosphérique

### 1. Les types de pollutions atmosphériques

Il existe plusieurs types de pollutions atmosphériques:

- Locale
  - Les particules fines (PM)
  - Les oxydes d'azote (NOx)
  - Le dioxyde de soufre (SO2)
  - Le monoxyde de carbone (CO)
  - Les composés organiques volatiles (COV)
  - Les hydrocarbures aromatiques polycycliques (HAP)
  - Les métaux
- Régionale:
  - L'ozone (O3)
- Globale:
  - Les gaz à effet de serre (exemple: CO2)

La pollution atmosphérique globale est causée par la pollution atmosphérique régionale, elle-même causée par la pollution atmosphérique locale. 

### 2. Les particules en suspension (aéroportées)

<p align="center"><img src="https://upload.wikimedia.org/wikipedia/commons/5/53/Airborne-particulate-size-chart_fr_particules.jpg" alt="Airborne-particulate-size-chart fr particules.jpg" height="720" width="658"></p>  

Les particules fines PM2.5 et PM10 sont des types de particules aéroportées:

- PM2.5 : dont le diamètre est inférieur à 2,5 micromètres, appelées « particules fines ». Ces particules peuvent pénétrer les alvéoles pulmonaires ce qui les rendent plus dangeureuses.
- PM10 : particules en suspension dans l'air, d'un diamètre aérodynamique (ou diamètre aéraulique) inférieur à 10 micromètres.
Peuvent pénètrent les bronches

### 3. Les sources de contamination de l'air

- Les installations industrielles (SO2, poussières, métaux, COV, ...)
- Les transports (NOx, CO, PM, ...)
- L'incinération des ordures ménagères (Métaux, acide chlorhydrique, dioxines, ...)
- L'agriculture (Poussières, NH3, ...)
- Les activités domestiques (COV, ...)
- Le chauffage individuel et collectif (SO2, CO, NOx, PM, ...)

#### 4. Un indice de la qualité de l'air (IQA)

Indice ATMO pour les agglomérations de plus de 100 000 habitants et indice IQA pour les autres. L'indice IQA est un indice simplifié qui repose sur la mesure d'un nombre réduit de polluants.
Les polluants mesurés sont l'ozone, le dioxyde de soufre, le dioxyde d'azote, les PM10 et PM2.5.

## II. Expérimentation, résultats et analyse

Pour mesurer la quantité de particules fines dans l'air, nous utilisons le capteur SDS011. La mesure de la concentration en monoxyde de carbone est assurée par le capteur MQ-9.  
La mesure de la quantité de particules fines par le SDS011 repose sur le principe physique de la diffusion laser. La résolution du diamètre des particules est d'au moins 0.3µm. L'erreur relative de mesure est d'environ 10%. Les données sont sauvegardées dans un fichier CSV sur une carte microSD. L'interval entre les mesures est de 5 secondes.  

Les données sauvegardées sont:

- Les coordonnées GPS de la mesure (latitude, longitude, altitude)
- La température
- L'hygrométrie relative
- La pression relative
- La masse par unité de volume d'air de particules fines <2.5 microns
- La masse par unité de volume d'air de particules fines <10 microns
- La concentration de monoxyde de carbone (CO) en ppm

Les mesures sont réalisés à différents lieux:

- Dans une salle de classe
- Dans une maison
- Dans une forêt (nature)
- En ville
- A proximité de grands axes routiers

L'objectif est de comparer les mesures suivant le lieu et de conclure sur l'impact des activités humaines dans les variations des mesures obtenues.

## III. Conclusion

___
Sources:

- Wikipédia
- acnusa.fr
