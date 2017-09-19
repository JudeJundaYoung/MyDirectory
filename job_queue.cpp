#include <iostream>
#include <vector>
#include <algorithm>
#include<utility>
#include<random>


using std::vector;
using std::cin;
using std::cout;
using std::pair;
using std::make_pair;
using std::swap;

static int count = 0;


class JobQueue {
 private:
  int num_workers_;
  vector<int> jobs_;

  vector<int> assigned_workers_;
  vector<int> assigned_workers_naive;
  vector<long long> start_times_;
  vector<long long> start_times_naive;

  void WriteResponse() const {
    for (int i = 0; i < jobs_.size(); ++i) {
      cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
    }
  }

  void ReadData() {
    int m;
    cin >> num_workers_ >> m;
	int temp;
	for (int i = 0; i < m; ++i)
	{
		cin >> temp;
		jobs_.push_back(temp);
	}
	return;
  }

  void ReadData_press_test()
  {
	  std::uniform_int_distribution<unsigned> u0(6,10);
	  std::uniform_int_distribution<unsigned> u1(1, 2);
	  std::default_random_engine e(count++);
	  num_workers_ = u0(e);
	  int m = u0(e);
	  int temp;
	  for (int i = 0; i < m; ++i)
	  {
		  temp = u1(e);
		  jobs_.push_back(temp);
	  }
	  return;
  }

  void AssignJobs() 
  {
	  // TODO: replace this code with a faster algorithm.
	  vector<pair<long long, int>> processors;
	  for (int i = 0 ; i < num_workers_; i++)
	  {
		  processors.emplace_back(make_pair(0, i));
	  }
	  for (int i = 0; i < jobs_.size(); i++)
	  {
		  start_times_.push_back(processors[0].first);
		  assigned_workers_.push_back(processors[0].second);
		  processors[0].first += jobs_[i];
		  sift_down(processors, 0);
	  }
	  
  }
  int find_min_first(vector<pair<long long, int>> vec, int root)
  {
	  int left = root;
	  int right = root;
	  if (2 * root + 1 <= vec.size() - 1)
		  left = 2 * root + 1;
	  if (2 * root + 2 <= vec.size() - 1)
		  right = 2 * root + 2;
	  if (left == right)
		  return root;
	  else
		  return compare_first(vec, root, left, right);
		
  }
  int find_min_second(vector<pair<long long, int>> vec, int root)
  {
	  int left;
	  int right;
	  if (root * 2 + 1 <= vec.size() - 1 && vec[root * 2 + 1].first == vec[root].first)
		  left = find_min_second(vec,root*2 + 1);
	  else
	  {
		  left = root;
	  }
	  if (root * 2 + 2 <= vec.size() - 1 && vec[root * 2 + 2].first == vec[root].first)
		  right = find_min_second(vec, root * 2 + 2);
	  else
	  {
		  right = root;
	  }
	  return compare_second(vec, root, left, right);
  }
  int compare_first(vector<pair<long long, int>> vec, int root, int left, int right)
  {
	  int rootval = vec[root].first;
	  int lval = vec[left].first;
	  int rval = vec[right].first;
	  int minval = std::min(rootval, std::min(lval, rval));
	  if (minval == rootval)
	  {
		  return root;
	  }
	  if (minval == lval)
		  return left;
	  if (minval == rval)
		  return right;
	  return root;
  }
  int compare_second(vector<pair<long long, int>> vec, int root, int left, int right)
  {
	  int rootval = vec[root].second;
	  int lval = vec[left].second;
	  int rval = vec[right].second;
	  int minval = std::min(rootval, std::min(lval, rval));
	  if (minval == rootval)
		  return root;
	  if (minval == lval)
		  return left;
	  if (minval == rval)
		  return right;
	  return root;
  }
  void sift_down_by_first(vector<pair<long long, int>> vec, int root)
  {
	  int left = (root * 2 + 1 <= vec.size() - 1) ? root * 2 + 1 : root;
	  int right = (root * 2 + 2 <= vec.size() - 1) ? root * 2 + 2 : root;
	  int min_first_indict = find_min_first(vec, root);
	  if (min_first_indict == left);
	  {
		  swap(vec[root], vec[left]);
		  sift_down_by_first(vec, left);
	  }
	  if (min_first_indict == right)
	  {
		  swap(vec[root], vec[left]);
		  sift_down_by_first(vec, right);
	  }
	  return;
  }
  void sift_down_by_second(vector<pair<long long, int>> vec, int root)
  {
	  int min_second_indict = find_min_second(vec, root);
	  if (min_second_indict == root)
	  {
		  return;
	  }
	  else
	  {
		  swap(vec[root], vec[min_second_indict]);
		  return;
	  }
  }
  void sift_down(vector<pair<long long, int>> vec, int root)
  {
	  sift_down_by_first(vec, root);
	  sift_down_by_second(vec, root);
  }


void WriteResponse_naive() const {
	for (int i = 0; i < jobs_.size(); ++i) {
		cout << assigned_workers_naive[i] << " " << start_times_naive[i] << "\n";
	}
}
void AssignJobs_naive() {
	// TODO: replace this code with a faster algorithm.
	assigned_workers_naive.resize(jobs_.size());
	start_times_naive.resize(jobs_.size());
	vector<long long> next_free_time(num_workers_, 0);
	for (int i = 0; i < jobs_.size(); ++i) {
		int duration = jobs_[i];
		int next_worker = 0;
		for (int j = 0; j < num_workers_; ++j) {
			if (next_free_time[j] < next_free_time[next_worker])
				next_worker = j;
		}
		assigned_workers_naive[i] = next_worker;
		start_times_naive[i] = next_free_time[next_worker];
		next_free_time[next_worker] += duration;
	}
}

bool press_test()
{
	if (start_times_ != start_times_naive || assigned_workers_ != assigned_workers_naive)
	{
		std::cout << "wrong occured!"<<std::endl;
		return 0;
	}
	return 1;
}

 public:
  void Solve() 
  {
	  int judge = 1;
	  do
	  {
		  jobs_.clear();
		  assigned_workers_.clear();
		  assigned_workers_naive.clear();
		  start_times_.clear();
		  start_times_naive.clear();
		  ReadData_press_test();
		  AssignJobs();
		  AssignJobs_naive();
		  WriteResponse();
		  WriteResponse_naive();
		  std::cout << "\n";
		  judge = press_test();
	  } while (judge);

  }
};

int main() {
  std::ios_base::sync_with_stdio(false);

  JobQueue job_queue;
  job_queue.Solve();
  system("pause");
  return 0;
}


