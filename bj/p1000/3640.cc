#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;
struct Mcmf{
	struct Edge{
		int v, capa, dist;
		Edge* rev;
		Edge(int v, int capa, int dist) : v(v), capa(capa), dist(dist) 	{}
	};

	int n, s, t;
	vector<vector<Edge*> > e;
	vector<int> chk, d;
	vector<pair<int, int> > pred;

	Mcmf(int n, int s, int t): n(n), s(s), t(t){
		e.resize(n), chk.resize(n);
		d.resize(n), pred.resize(n);
	}

	void add_edge(int u, int v, int capa, int dist){
		Edge* edge = new Edge(v, capa, dist);
		Edge* res = new Edge(u, 0, -dist);
		edge->rev = res;
		res->rev = edge;
		e[u].push_back(edge), e[v].push_back(res);
	}

	bool spfa(int& flow, int& cost){
		fill(d.begin(), d.end(), 0x3f3f3f3f);
		fill(chk.begin(), chk.end(), 0);
		fill(pred.begin(), pred.end(), make_pair(-1, -1));

		queue<int> q;
		q.push(s);
		d[s]=0;

		while(!q.empty()){
			int now = q.front(); q.pop();
			chk[now]=0;

			for (int i=0; i<e[now].size(); i++){
				if (e[now][i]->capa == 0) continue;

				int v = e[now][i]->v, dist = e[now][i]->dist;
				if ( d[v] > d[now] + dist){
					d[v] = d[now] +dist;
					pred[v] = make_pair(now, i);
					if (!chk[v] ) q.push(v), chk[v]=1;
				}
			}
		}

		if (d[t] == 0x3f3f3f3f) return 0;

		int now =t;
		int mn = 0x3f3f3f3f;

		while(pred[now].first != -1){
			mn = min(e[pred[now].first][pred[now].second]->capa, mn);
			now = pred[now].first;
		}

		now = t;

		while(pred[now].first != -1){
			e[pred[now].first][pred[now].second]->capa -=mn;
			e[pred[now].first][pred[now].second]->rev->capa +=mn;
			now = pred[now].first;
		}

		flow += mn;
		cost += mn*d[t];
		return 1;
	}

	int flow(){
		int flow=0, cost=0;
		while(spfa(flow, cost));
		return cost;
	}
};

int main(){
	int n, m;
	while(~scanf("%d %d", &n, &m)){

		Mcmf mf(n*2+2, 0, n*2+1);
		mf.add_edge(0, 1, 2, 0);
		mf.add_edge(n*2, n*2+1, 2, 0);
		mf.add_edge(1, 2, 2, 0);
		mf.add_edge(n*2-1, 2*n, 2, 0);
		for (int i=2; i<=n-1; i++) mf.add_edge(i*2-1, i*2, 1, 0);

		while(m--){
			int u, v, d; scanf("%d %d %d", &u, &v, &d);
			mf.add_edge(u*2, v*2-1, 1, d);
		}

		printf("%d\n", mf.flow());
	}
}
