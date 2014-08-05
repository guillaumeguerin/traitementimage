package aquarium.factory;

import java.awt.Point;

import aquarium.gui.Aquarium;
import aquarium.items.Stone;
import aquarium.util.RandomNumber;

public class StoneFactory extends AquariumItemFactory<Stone> {

	@Override
	public Stone newItems() {
		Stone s = new Stone(RandomNumber.randomValue(Stone.getMinWidth(), Stone.getMaxWidth()));
		s.setPosition(RandomNumber.randomPoint(0, Aquarium.SIZE_AQUA_X-Stone.getMaxWidth(), 0, Aquarium.SIZE_AQUA_Y-Stone.getMaxWidth()));
		return s;
	}

	public Stone newItems(int taille, int posX, int posY) {
		Stone s = new Stone(taille);
		s.setPosition(new Point(posX, posY));
		return s;
	}
	
}
