public interface DataManipulation {

    public void openDatasource();
    public void closeDatasource();
    public int addOneMovie(String str);
    public String allContinentNames();
    public String continentsWithCountryCount();
    public String FullInformationOfMoviesRuntime(int min, int max);
    public String findMovieById(int id);
}
