package aquarium.items;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.image.ImageObserver;
import java.util.Collection;

public abstract class AquariumItem {

	private Image image = null;
	
	protected Point position = null;
	protected int width = -1;
	protected int height = -1;

	public AquariumItem(int width, Image image) {
		this.image = image;		
		this.width = width;
		
		setHeight(width);
		
		/*
		 * Les images sont chargées de façon asynchrone, on doit donc
		 * attendre que les dimensions soient chargées pour les
		 * récupérer.
		 */
		/*image.getWidth(new ImageObserver() {
			@Override
			public boolean imageUpdate(Image img, int infoflags, int x, int y,
					int w, int h) {
				if((infoflags & WIDTH) == 0 || (infoflags & HEIGHT) == 0)
					return false;
				setHeight(getWidth() * h / w);
				return true;
			}
		});*/
	}

	//Dessine l'image pour une item héritant de AquariumItem
	public void draw(Graphics g) {
		g.drawImage(image, position.x, position.y, width, height, null);
	}
	
	public Point getPosition(){
		return position;
	}
	
	public void setPosition(Point p) {
		position = p;
	}
	
	public int getWidth() {
		return width;
	}
	
	private void setHeight(int h) {
		height = h;
	}
	
	// retourne le rectangle englobant
	public Rectangle rectangle() {
		return new Rectangle(position.x, position.y, width, height);
	}

	/*
	 * parcourt la collection et retourne vrai dès  
	 * que le rectangle de cet item en coupe un autre 
	 * faux à la fin du parcours (aucune intersection)
	 */
	public boolean intersects(Collection<AquariumItem> c) {
		for(AquariumItem ai : c) {
			if(ai.rectangle().intersects(rectangle()))
				return true;
		}
		return false;
	}

}