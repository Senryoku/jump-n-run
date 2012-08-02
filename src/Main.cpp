/**
 @mainpage
 <a name="Présentation"></a>Présentation<a href="#Pr%C3%A9sentation" class="section_anchor"></a></h2><p>C'est un jeu de courses avec un bonhomme. Un éditeur de niveau permet de créer de très larges mondes avec plein d'obstacles.  La physique avancée permet de gérer des polygones fixes ou pas, des  points et des contraintes rigides et élastiques (ressorts). On peu aussi importer des images personnalisées pour créer n'importe quel objet et le mettre dans le niveau.  </p><h2><a name="Captures_d'écran"></a>Captures d'écran<a href="#Captures_d%27%C3%A9cran" class="section_anchor"></a></h2><p><img src="http://i.imgur.com/yZ9MJ.png"> </p><p>Une chaîne de boîtes liées par de ressorts </p><p><img src="http://i.imgur.com/wtdkd.png"> </p><p>Quelques objets avec des images personalisées </p><p><img src="http://i.imgur.com/AmhKz.png"> </p><p>Le joueur en affichage Debug et un Menu de test </p><p><img src="http://i.imgur.com/jxxQE.png"> <img src="http://i.imgur.com/8tuFG.png"> </p><p>Le joueur en affichage normal </p><h2><a name="Compilation"></a>Compilation<a href="#Compilation" class="section_anchor"></a></h2><p>Pour pouvoir compiler il est nécessaire d'avoir SFML 2 et OpenGL Vous pouvez utiliser soit la version RC de SFML 2.0 soit la version GitHub<a href="/p/jump-n-run/w/edit/GitHub">?</a>. On encourage d'utiliser cette dernière qui est d'ailleurs très facile à installer grâce à CMake. </p><p><a href="https://github.com/LaurentGomila/SFML" rel="nofollow">https://github.com/LaurentGomila/SFML</a> </p><p>Pour compiler SFML 2 voici une aide avec les dépendences : <a href="http://sfmlcoder.wordpress.com/2011/08/16/building-sfml-2-0-with-make-for-gcc/" rel="nofollow">http://sfmlcoder.wordpress.com/2011/08/16/building-sfml-2-0-with-make-for-gcc/</a> </p><p><a href="http://www.sfml-dev.org/download.php#2.0-rc" rel="nofollow">http://www.sfml-dev.org/download.php#2.0-rc</a> </p><p>Le binaire est créé dans bin/ et doit être exécuté depuis . </p><p><strong>Unix</strong> </p><p><i>Uniquement testé sous OS X 10.6, Linux Ubuntu 11 et Kubuntu 12</i> Récupérer le code et se placer dans le répertoire de travail </p><pre class="prettyprint"><span class="pln">svn checkout http</span><span class="pun">:</span><span class="com">//jump-n-run.googlecode.com/svn/trunk/ jump-n-run-read-only</span><span class="pln"><br>cd jump</span><span class="pun">-</span><span class="pln">n</span><span class="pun">-</span><span class="pln">run</span><span class="pun">-</span><span class="pln">read</span><span class="pun">-</span><span class="pln">only</span></pre><p>Lancer le jeu </p><pre class="prettyprint"><span class="pln">make runGame</span></pre><p>Lancer l'éditeur </p><pre class="prettyprint"><span class="pln">make runEditor</span></pre><p><strong>Windows</strong> Utiliser de préférence le projet CodeBlocks<a href="/p/jump-n-run/w/edit/CodeBlocks">?</a> JumpnRun<a href="/p/jump-n-run/w/edit/JumpnRun">?</a>.cbp Sinon faire juste la première fois: </p><pre class="prettyprint"><span class="pln">make windirs</span></pre><p>Et pour lancer utiliser les mêmes commandes que pour Unix </p><h2><a name="Contrôles"></a>Contrôles<a href="#Contr%C3%B4les" class="section_anchor"></a></h2><p>Regarder dans readme.txt </p><h2><a name="Crédits"></a>Crédits<a href="#Cr%C3%A9dits" class="section_anchor"></a></h2><p>Développé par <strong>Yoann Maret-Verdant</strong> et <strong>Eduardo San Martin Morote</strong>
 */

#include <Game/Game.h>

int main(int argc, char** argv)
{
	if (!DirectoryExists("replays"))
		CreateDirectory("replays");
	
	sf::Context C;
	C.setActive(1);
	SharedResources SR;
	shInit(&SR, &glTexLoad, &glTexFree);
	shLoadAudio(&SR);
	shLoadTextures(&SR);
	shLoadFonts(&SR);

	Game G;

	gmInit(&G, &SR);
	gmLoadLvl(&G, (ResourcePath()+"levels/MainMenu.lvl").c_str());
	gmPlay(&G);
	gmFree(&G);

	shFree(&SR);

	return EXIT_SUCCESS;
}
