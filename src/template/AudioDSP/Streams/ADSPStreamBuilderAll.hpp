class UnixBuilder: public Builder
{
public:
  UnixBuilder()
  {
      _result = new DistrWorkPackage("Unix");
  }
  void configureFile(char *name)
  {
      _result->setFile("flatFile", name);
  }
  void configureQueue(char *queue)
  {
      _result->setQueue("FIFO", queue);
  }
  void configurePathway(char *type)
  {
      _result->setPathway("thread", type);
  }
};
