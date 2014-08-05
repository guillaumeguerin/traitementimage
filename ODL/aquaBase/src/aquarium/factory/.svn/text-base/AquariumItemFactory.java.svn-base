package aquarium.factory;

import java.util.Collection;

import aquarium.gui.Aquarium;
import aquarium.items.AquariumItem;
import aquarium.util.RandomNumber;

public abstract class AquariumItemFactory< T extends AquariumItem> {

	public abstract T newItems();

	public void sink(Collection<AquariumItem> items, AquariumItem instance) {
		while(instance.intersects(items)) {
			instance.setPosition(RandomNumber.randomPoint(0, Aquarium.SIZE_AQUA_X, 0, Aquarium.SIZE_AQUA_Y));
		}
	}

}
