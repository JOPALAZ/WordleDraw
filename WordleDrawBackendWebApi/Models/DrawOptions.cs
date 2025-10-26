namespace WordleDrawBackendWebApi.Models
{
    public enum ValidatorType
	{
		PerferctValidator,
		PresenceValidator,
		MixedValidator,
		FallbackPresenceValidator,
		FallbackMixedValidator,
		AtLeastGreenValidator,
		AtLeastYellowValidator,
		NotSpecified
	};

    public enum Color
	{
		Black,
		Yellow,
		Green
	};

	public class DrawOptions
    {
        public string enumValue { get; set; }
		public string answer { get; set; }
        public ICollection<ICollection<int>> grid { get; set; }
    }

	public class ColorRequest 
	{
		public string answer { set; get; }
		public ICollection<string> words { set; get; }
	}
}
