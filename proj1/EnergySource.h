//EnergySource.h

#ifndef ENERGYSOURCE_INCLUDED
#define ENERGYSOURCE_INCLUDED
class EnergySource
{
  public:
        // Constructor
    EnergySource(int r, int c);

        // Accessors
    int row() const;
    int col() const;

  private:
    int m_row;
    int m_col;
};
#endif //ENERGYSOURCE_INCLUDED