package application;

public class nextMusicFile {
	public static void next(){
    	if (Main.musicIsPlaying == true){
    		Main.mediaPlayer.stop();
    	}
    	
    	if (Main.randomTrack == true){
    		if(Main.maxTrackCount != 0){
    			if(Main.currentTrackID == Main.maxTrackCount-1){
        			for(int i = 0; i<50; i++){
        				System.out.println(Main.musicPathList);
        				System.out.println("Neue Liste wird generiert");
        				
        				String shuffle1 = "";
        				String shuffle2 = "";
        				String shuffle3 = "";
        				
        				System.out.println(shuffle1);
        				System.out.println(shuffle2);
        				System.out.println(shuffle3);
        				
        				System.out.println(Main.maxTrackCount);
        				
        				int ID1 = Main.randomTrackID.nextInt(Main.maxTrackCount);
        				shuffle1 = Main.musicPathList[ID1];
        				System.out.println(ID1);
        				int ID2 = Main.randomTrackID.nextInt(Main.maxTrackCount);
        				shuffle2 = Main.musicPathList[ID2];
        				System.out.println(ID2);
        				shuffle3 = shuffle2;
        				shuffle2 = shuffle1;
        				shuffle1 = shuffle3;
        				Main.currentTrackID = 0;
        			}
        			Main.currentTrackID = 0;
        			//System.out.println("Next Random Track: " + currentTrackID);
        		}
        		else{
        			Main.currentTrackID++;
        			//currentTrackID = 0;
        		}
    		}
    		
    	}
    	else{
    		if (Main.currentTrackID < Main.maxTrackCount - 1){
    			Main.currentTrackID ++;
        	}
        	else{
        		Main.currentTrackID = 0;
        	}
    	}
    	/*
    	if (randomTrack == true){
    		if(maxTrackCount != 0){
    			currentTrackID = randomTrackID.nextInt(maxTrackCount);
    			System.out.println("Next Random Track: " + currentTrackID);
    		}
    		else{
    			currentTrackID = 0;
    		}
    	}
    	else{
    		if (currentTrackID < maxTrackCount - 1){
        		currentTrackID ++;
        	}
        	else{
        		currentTrackID = 0;
        	}
    	}
    	*/
    	
    	MPlayer.play();
    }
}