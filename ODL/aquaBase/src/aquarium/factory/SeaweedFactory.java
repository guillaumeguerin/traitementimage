package aquarium.factory;

import java.awt.Point;

import aquarium.gui.Aquarium;
import aquarium.items.Seaweed;
import aquarium.items.Stone;
import aquarium.util.RandomNumber;

public class SeaweedFactory extends AquariumItemFactory<Seaweed> {
	
	@Override
	public Seaweed newItems() {
		Seaweed sw = new Seaweed(RandomNumber.randomValue(Seaweed.getMinWidth(), Seaweed.getMaxWidth()));
		sw.setPosition(RandomNumber.randomPoint(0, Aquarium.SIZE_AQUA_X-Seaweed.getMaxWidth(), 0, Aquarium.SIZE_AQUA_Y-Seaweed.getMaxWidth()));
		return sw;
	}

	public Seaweed newItems(int taille, int posX, int posY) {
		Seaweed s = new Seaweed(taille);
		s.setPosition(new Point(posX, posY));
		return s;
	}
	
}
