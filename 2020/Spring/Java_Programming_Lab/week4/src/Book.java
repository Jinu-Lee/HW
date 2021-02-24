public class Book {

	private boolean isBorrowed = false;
	
	boolean checkBorrowed() {
		return isBorrowed;
	}
	
	void mark() {
		isBorrowed = !isBorrowed;
	}
	
	public static void main(String[] args) {
		
	}
}
