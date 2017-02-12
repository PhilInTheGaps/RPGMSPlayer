package application;

import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.util.ArrayList;
import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.Region;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class Main extends Application {
	
	//Defining Variables
	Scene scene;
	BorderPane borderPane;
	double height;
	double width;
	public static ArrayList<String> settings = new ArrayList<String>();
		
	//Main
	public static void main(String[] args){
		
		launch(args);
	}
	
	//Start of Program
	@Override
	public void start(Stage primaryStage) {
        
		System.out.println("Initializing...");
		System.out.println("");
		
        scene = setScene(this.width, this.height);
        scene.setFill(Color.BLACK);

        primaryStage.setTitle("RPG Music and Sound Player | © 2016-2017 Phil Hoffmann, Niklas Lüdtke | Version 0.2.3 Beta");
        primaryStage.setScene(scene);
        primaryStage.show();
        primaryStage.setMaximized(true);
        UI.defaultWidth = (double) scene.getWidth();
        adjustUI();
        
        
	}
	
	//Defining Scene
	public Scene setScene(double width, double height){
		
		borderPane = new BorderPane();
		
		//Check OS
		checkOS();
		
		//Read Settings
        String sp = "settings.txt";
        System.out.println("Reading Settings...");
        try (LineNumberReader br = new LineNumberReader(new FileReader(sp))) {
        	for(String line = null; (line = br.readLine()) != null;){
        		//System.out.println(line);
        		//System.out.println(settings.size());
        		settings.add(line);
        	}

		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Reading Settings Failed");
		}
        System.out.println("Finished Reading Settings");
        System.out.println("");
		
		//Add Components
        borderPane = new BorderPane();
        borderPane.setTop(UI.addToolBar());
        borderPane.setStyle("-fx-background-color: White");
        borderPane.setCenter(UI.tabPane);	
		borderPane.setLeft(UI.addVBox());
		try {
			borderPane.setBottom(UI.addBotBox());
		} catch (IOException e) {
			e.printStackTrace();
		}
		UI.addTabPane();
		
        scene = new Scene(borderPane, 1280, 720);
        scene.setFill(Color.WHITE);
        UI.defaultWidth = (double) scene.getWidth();
        adjustUI();
        
        scene.widthProperty().addListener(new ChangeListener<Number>() {
            @Override 
            public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneWidth, Number newSceneWidth) {
            	//System.out.println("Width: " + newSceneWidth);
            	UI.defaultWidth = (double) newSceneWidth;
            	adjustUI();
            }
        });
        
        scene.heightProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneHeight, Number newSceneHeight) {
                //System.out.println("Height: " + newSceneHeight);
            	UI.defaultHeight = (double) newSceneHeight;
            	//UI.tile.setPrefHeight(UI.defaultHeight);
                
            }
        });
        
        scene.setOnKeyPressed(
    		new EventHandler<KeyEvent>(){
		         @Override
		         public void handle(KeyEvent keyEvent){
		        	 if (keyEvent.getCode() == KeyCode.F1) {
	                    System.out.println("Dev Mode Activated!");
	                    if(UI.devV){
	                    	UI.devV = false;
	                    	UI.debug = false;
	                    	borderPane.setLeft(UI.addVBox());
	                    }
	                    else{
	                    	UI.devV = true;
	                    	borderPane.setLeft(UI.addVBox());
	                    }
    	 			}
	         	}	
			}
		);
        
        
        
        return scene;
        
	}
  	
	//Adjusting UI
	public static void adjustUI(){
        
		/*
    	UI.defaultMusicAndSoundWidth = UI.defaultWidth-2*UI.defaultPadding-UI.defaultSliderWidth;
    	//System.out.println("Tabpane Width: " + UI.defaultMusicAndSoundWidth);
    	
    	
        UI.tile.setPrefWidth(UI.defaultMusicAndSoundWidth);
        UI.tile.setMinWidth(UI.defaultMusicAndSoundWidth);
        UI.tile2.setPrefWidth(UI.defaultMusicAndSoundWidth);
        UI.tile2.setMinWidth(UI.defaultMusicAndSoundWidth);
        */
        UI.toolBar1.setPrefWidth(UI.defaultWidth);
        Object[] bArray1 = UI.toolBar1.getChildren().toArray();
        Object[] bArray2 = UI.toolBar2.getChildren().toArray();
        int bCount = bArray1.length;
        int bCount2 = bArray2.length;
        UI.defaultButtonWidth = UI.defaultWidth/bCount;
        /*
        Object[] bArrayMusic = UI.tile.getChildren().toArray();
        int bCountMusic = bArrayMusic.length;
        
        System.out.println("Button Count: " + bCountMusic);
        System.out.println("DefaultFolderButtonWidth: " + UI.defaultFolderButtonWidth);
        System.out.println("FolderButtonWidth: " + UI.folderButtonWidth);
        
        Object[] bArraySounds = UI.tile2.getChildren().toArray();
        int bCountSounds = bArraySounds.length;
        
        //Adjusting Music and Sound Button width
        double buttonsFittingIn = (UI.defaultMusicAndSoundWidth-(UI.buttonRowCount)*UI.defaultPadding)/UI.defaultFolderButtonWidth;
        double availableSpace = (UI.defaultMusicAndSoundWidth-2*UI.defaultPadding-(UI.buttonRowCount)*UI.defaultPadding/4);
        UI.buttonRowCount = (int) Math.floor(buttonsFittingIn);
        /*
        System.out.println("Buttons Fitting in: "+buttonsFittingIn);
        System.out.println("ButtonRowCount: "+UI.buttonRowCount);
        System.out.println("Available Space: "+availableSpace);
    	
        UI.folderButtonWidth = (buttonsFittingIn/(UI.buttonRowCount))*UI.defaultFolderButtonWidth-1;
        /*
        System.out.println("CurrentWidth: "+UI.folderButtonWidth);
        System.out.println("Space/Width: "+availableSpace/UI.folderButtonWidth);
    	
    	for(int i = 0; i < bCountMusic; i++){
    		((Region) bArrayMusic[i]).setPrefWidth(UI.folderButtonWidth);
    	}
    	for(int i = 0; i < bCountSounds; i++){
    		((Region) bArraySounds[i]).setPrefWidth(UI.folderButtonWidth);
    	}
    	
    	//System.out.println("");
    	*/
		
    	//Adjusting ToolBar button width
        for(int i = 0; i < bCount; i++){
        	((Region) bArray1[i]).setPrefWidth(UI.defaultButtonWidth);
        }
        for(int i = 0; i < bCount2; i++){
        	((Region) bArray2[i]).setPrefWidth(UI.defaultButtonWidth);
        }
        
        //Adjusting ProgressBar Width
        
        UI.pb.setPrefWidth(UI.defaultWidth);
	}
	
	@Override
	public void stop(){
	    System.out.println("Closing...");
	    UI.stopDownload = true;
	}
	
  	//Checking the Operating System
  	public static void checkOS(){
  		
  		UI.mainPath = Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
		
  		UI.osName = System.getProperty("os.name");
		System.out.println("Operating System: "+UI.osName);
		
		if(UI.osName.toLowerCase().contains("windows")){
			UI.windows = true;
			System.out.println("OS detected as Windows");
		}
		else if(UI.osName.toLowerCase().contains("linux")){
			UI.linux = true;
			System.out.println("OS detected as Linux");
		}
		else{
			System.out.println("OS currently not supportet, maybe it will work, maybe not.");
			UI.windows = true;
		}
		System.out.println("");
  	}
}
