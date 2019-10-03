#include <bits/stdc++.h>
#define DEBUG if(0)
#define lli long long int
#define ldouble long double
using namespace std;

const int maxN = 100, maxTurns = 1e3; int n;
struct Card
{
  int health, power;
  list<int> reachable;
  void read()
  {
    scanf("%d %d", &health, &power);
    int k; scanf("%d", &k);
    for (int i = 0; i < k; i ++)
    {
      int a; scanf("%d", &a);
      reachable.push_back(a);
    }
  }
};
Card cards[2][maxN]; int healthSum[2];

const int maxV = 201 + 1, inf = 1e9;
int source = 0, sink, vertices;
int level[maxV], ptr[maxV];
// source (0) -> Cards (1 : n) -[power]> Cards (n + 1 : 2*n) -[health]> sink (2*n + 1)
struct Edge { int to, back, flow, capacity; };
vector<Edge> graph[maxV];
void addEdge(int u, int v, int f)
{
  graph[u].push_back({v, (int) graph[v].size(), f, f});
  graph[v].push_back({u, (int) graph[u].size() - 1, 0, 0});
}

bool bfs()
{
  memset(level, -1, sizeof(level));
  queue<int> q; q.push(source), level[source] = 0;
  while (!q.empty())
  {
    int u = q.front(); q.pop();
    for (Edge &e: graph[u])
      if (e.flow && level[e.to] == -1)
        q.push(e.to), level[e.to] = level[u] + 1;
  }
  return level[sink] != -1;
}
int dfs(int u = source, int flow = inf)
{
  if (u == sink || !flow) return flow;
  for (int &p = ptr[u]; p < graph[u].size(); p ++)
  {
    Edge &e = graph[u][p];
    if (e.flow && level[e.to] == level[u] + 1)
    {
      int delivered = dfs(e.to, min(e.flow, flow));
      e.flow -= delivered;
      graph[e.to][e.back].flow += delivered;
      if (delivered) return delivered;
    }
  }
  return 0;
}
int dinic()
{
  int maxFlow = 0, flow;
  while (bfs())
  {
    memset(ptr, 0, sizeof(ptr));
    while (flow = dfs()) maxFlow += flow;
  }
  return maxFlow;
}

int can(int player, int turns)
{
  for (int i = 0; i < vertices; i ++) graph[i].clear();
  for (int i = 0; i < n; i ++) addEdge(source, 1 + i, turns * cards[player][i].power);
  for (int i = 0; i < n; i ++)
    for (int j: cards[player][i].reachable)
      addEdge(1 + i, n + 1 + j, turns * cards[player][i].power);
  for (int i = 0; i < n; i ++) addEdge(n + 1 + i, sink, cards[1 - player][i].health);
  return dinic() == healthSum[1 - player];
}

int binarySearch(int player)
{
  int lo = 1, hi = maxTurns;
  while (lo < hi)
  {
    int mid = (lo + hi) >> 1;
    if (can(player, mid)) hi = mid;
    else lo = mid + 1;
  }
  return lo;
}

int main()
{
  scanf("%d", &n);
  sink = 2*n + 1; vertices = sink + 1;
  for (int i = 0; i < 2; i ++)
    for (int j = 0; j < n; j ++)
    {
      cards[i][j].read();
      healthSum[i] += cards[i][j].health;
    }

  int entityTurns = binarySearch(0);
  int xTurns = binarySearch(1);
  printf("%s wins in %d turns\n", entityTurns <= xTurns ? "Entity" : "X", min(entityTurns, xTurns));

  return 0;
}