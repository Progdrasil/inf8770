# Question 01: Hypotheses
1. Pour un texte long LZW aura un meilleur taux de compression que Huffman. Par contre, si le texte est un ensemble aleatoire de symbole ou un texte court, Huffman sera meilleur.
2. Le temps de compression est proportionel aux taux de compression, donc dans les cas ou un codage a un taux plus eleve, la compression sera rapide.
3. Le taux de compression d'images brouillées sera meilleur pour le codage Huffman que pour le codage LZW.
4. Le taux de compression d'images uniformes ou avec peu de couleurs telles que des identicons sera meilleur avec LZW.
5. Le taux de compression de GIFs sera plus eleve pour le codage LZW que le codage Huffman.
6. Le temps de decompression est aussi proportionel au taux de compression. C'est a dire qu'un fichier avec un haut taux de compression sera plus rapide a decoder.

# Question 02: Methodologie
### La methode
* Pour chaque fichier dans notre base de donnees, on va compresser avec les deux types de codages, c'est a dire Huffman et LZW, et comparer le taux de compression et la vitesse de compression pour les deux fichiers en output.
### La base de donnees
* On aura des fichiers de texte avec differentes longueurs de deux types:
	* avec du texte comprehensible
	* avec un ensemble aleatoire de symboles
* On aura des images de differentes tailles de trois types:
	* Des photographies noir et blanc
	* Des images monochromes
	* Des images bruitees
	* Des images type 8-bit (identicons, 8bit art, etc)
* On aura des GIFs de differentes longueurs.

### Les criteres
* Les codes seront compares selon le taux de compression, celui avec le taux le plus eleve est considere meilleur pour cette tache.
* Les codes seront aussi compares selon la vitesse a laquelle ils sont compresses et decompresses. Celui qui est le plus rapide aux deux taches est considere meilleur pour cette tache.

### Code informatique utilise
Langage informatique?
* On utilise le c++ comme langage pour la rapidite de traitement de donnees.

# Question 03
CODE ALL THE THINGS!!!

# Question 04
ANALYSE ALL THE THINGS!!!


pour git clone fait cette commande:
``` bash
git submodule init --quiet
```
