package aquarium.items;

import java.awt.Image;
import java.awt.Point;
import java.util.Collection;

import aquarium.move.Mobile;

public abstract class MobileItem extends AquariumItem implements Mobile {

	private Point destination = null;

	public MobileItem(int width, Image image) {
		super(width, image);
	}

	// Doit être fournie par la classe concrète pour choisir la destination
	abstract public Point target(Collection<AquariumItem> neighbours);

	// Déplacement vers une destination donnée
	public boolean move(Point destination) {
		this.destination = destination;
		if(!destination.equals(position)) {
			int v = 200/this.width;
			int dx = (destination.x-position.x);
			int dy = (destination.y-position.y);
			int dist = (int)Math.sqrt(dx*dx+dy*dy);
			int modx = v*dx/dist;
			int mody = v*dy/dist;

			if (Math.abs(modx) > Math.abs(dx))
				modx = dx;

			if (Math.abs(mody) > Math.abs(dy))
				mody = dy;

			position.translate(modx,mody);				
			return true;
		}		
		return false;
	}

	public Point getCurrentDestination() {
		return destination;
	}
	
	public void setCurrentDestination(Point p){
		this.destination = p;
	}

	//**************************
	// Implement Mobile
	//**************************

	// Choisit une destination et y va. Lorsque parvenu à la destination,
	// en choisit une autre, etc.
	public void move(Collection<AquariumItem> neighbours) {
		if(destination == null | position.equals(destination))
			destination = target(neighbours);
		move(destination);	
	}
}
