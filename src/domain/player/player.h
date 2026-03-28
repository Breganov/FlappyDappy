class Player {
  public:
    const PlayerId& GetId() const;
    const std::string& GetNickname() const;
  private id_;
  std::string nickname_;
};
